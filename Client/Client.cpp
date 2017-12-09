#include "Client.h"
#include <QsLog/QsLog.h>
#include <QtCore/QTimer>
#include <QtNetwork/QHttpMultiPart>
#include <Client/Model/VenuePictures.h>

Client::Client(QObject *parent):
    QObject(parent),
    m_autoRefreshEnabled(true),
    m_venuePictures(new VenuePictures(this))
{
    QLOG_TRACE() << "Client::Client";
}

void Client::setServer(const QString &server)
{
    m_server = server;
}

QString Client::server() const
{
    return m_server;
}

void Client::setTokenAutoRefreshEnabled(bool enabled)
{
    m_autoRefreshEnabled = enabled;
}

bool Client::isTokenAutoRefreshEnabled() const
{
    return m_autoRefreshEnabled;
}

void Client::setAutoFetchPicturesEnabled(bool enabled)
{
    m_venuePictures->setAutoFetchPicturesEnabled(enabled);
}

bool Client::isAutoFetchPicturesEnabled() const
{
    return m_venuePictures->isAutoFetchPicturesEnabled();
}

Client::Status Client::status() const
{
    Status status = Offline;

    if (token().isValid()) {
        status = Online;
    }
    if (venue().isValid()) {
        return Ready;
    }

    return status;
}

JwtToken Client::token() const
{
    return m_token;
}

void Client::setVenue(const QString &venueToken)
{
    QLOG_TRACE() << "Client::setVenue(" << venueToken << ")";

    m_endpointGetVenue.setRequestParameters(server(), QStringList() << venueToken);
    get(addAuthHeader(m_endpointGetVenue.createRequest()));
}

Venue Client::venue() const
{
    return m_venue;
}

void Client::acquireToken(const QString &username, const QString &password)
{
    QLOG_TRACE() << "Client::acquireToken";

    m_endpointAcquireToken.setRequestParameters(server(), QStringList() << username << password);
    post(m_endpointAcquireToken.createRequest(), m_endpointAcquireToken.payload());
}

void Client::postPicture(const QImage &image)
{
    QLOG_TRACE() << "Client::postPicture";

    m_endpointPostPicture.setRequestParameters(server(), QStringList());
    post(addAuthHeader(m_endpointPostPicture.createRequest()), m_endpointPostPicture.payload(image));
}

void Client::linkPicture(const QString &publicPictureLocation)
{
    QLOG_TRACE() << "Client::linkPicture(" << publicPictureLocation << ")";

    QStringList parameters;
    parameters << QString::number(venue().id())
               << publicPictureLocation;

    m_endpointLinkPicture.setRequestParameters(server(), parameters);
    post(addAuthHeader(m_endpointLinkPicture.createRequest()), m_endpointLinkPicture.payload());
}

void Client::getVenuePictures()
{
    QLOG_TRACE() << "Client::getVenuePictures()";

    m_endpointGetVenuePictures.setRequestParameters(server(), QStringList() << QString::number(venue().id()));
    get(addAuthHeader(m_endpointGetVenuePictures.createRequest()));
}

void Client::requestFinished()
{
    QLOG_TRACE() << "Client::requestFinished()";

    auto *reply = dynamic_cast<QNetworkReply*>(sender());
    if (!reply) {
        return;
    }

    if (m_endpointAcquireToken.isMatch(reply)) {
        m_endpointAcquireToken.parseResponse(reply);
        setToken(m_endpointAcquireToken.token());
    } else if (m_endpointGetVenue.isMatch(reply)) {
        m_endpointGetVenue.parseResponse(reply);
        setVenue(m_endpointGetVenue.venue());
    } else if (m_endpointPostPicture.isMatch(reply)) {
        m_endpointPostPicture.parseResponse(reply);
        emit pictureUploaded(m_endpointPostPicture.publicLocation());
        linkPicture(m_endpointPostPicture.publicLocation());
    } else if (m_endpointLinkPicture.isMatch(reply)) {
        m_endpointLinkPicture.parseResponse(reply);
        emit pictureLinked();
    } else if (m_endpointGetVenuePictures.isMatch(reply)) {
        m_endpointGetVenuePictures.parseResponse(reply);
        m_venuePictures->readUpdate(m_endpointGetVenuePictures.venuePictures());
    }
}

void Client::requestError(QNetworkReply::NetworkError code)
{
    QLOG_TRACE() << "Client::requestError()" << code;
}

void Client::requestSslErrors(const QList<QSslError> &errors)
{
    QLOG_TRACE() << "Client::requestSslErrors()";
}

void Client::setToken(const JwtToken &token)
{
    m_token = token;
    emit tokenChanged(m_token);

    if (m_token.isValid()) {
        emit acquireTokenSucceed();
    } else {
        emit acquireTokenFailed();
    }

    updateStatus();
}

void Client::setVenue(const Venue &venue)
{
    m_venue = venue;
    emit venueChanged(m_venue);

    updateStatus();
}

void Client::updateStatus()
{
    QLOG_TRACE() << "Client::updateStatus()" << status();

    emit statusChanged(status());
}

QNetworkReply *Client::post(const QNetworkRequest &request, const QJsonObject &payload)
{
    return post(request, QJsonDocument(payload));
}

QNetworkReply *Client::post(const QNetworkRequest &request, const QJsonDocument &payload)
{
    return post(request, payload.toJson());
}

QNetworkReply *Client::post(const QNetworkRequest &request, const QByteArray &payload)
{
    QLOG_TRACE() << "Client::post()" << request.url() << payload;

    dumpRequestInfo("POST", request);
    return prepareReply(m_qnam.post(request, payload));
}

QNetworkReply *Client::post(const QNetworkRequest &request, QHttpMultiPart *payload)
{
    QLOG_TRACE() << "Client::post()" << request.url() << "(HttpMultiPart)";

    dumpRequestInfo("POST", request);
    return prepareReply(m_qnam.post(request, payload));
}

QNetworkReply *Client::get(QNetworkRequest request)
{
    QLOG_TRACE() << "Client::get()" << request.url();

    dumpRequestInfo("GET", request);
    return prepareReply(m_qnam.get(request));
}

QNetworkRequest Client::addAuthHeader(QNetworkRequest request)
{
    if (token().isLifetimeExpired()) {
        QLOG_ERROR() << "a valid token has to be acquired prior to using it";
        return request;
    }

    request.setRawHeader("Authorization", QString("Bearer %1").arg(m_token.token()).toUtf8());

    return request;
}

QNetworkReply *Client::prepareReply(QNetworkReply *reply)
{
    connect(reply, &QNetworkReply::finished, this, &Client::requestFinished);
    connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
            this, &Client::requestError);
    connect(reply, &QNetworkReply::sslErrors, this, &Client::requestSslErrors);

    return reply;
}

void Client::dumpRequestInfo(const QString &verb, const QNetworkRequest &request) const
{
    QLOG_DEBUG() << verb << request.url().toString()/* << request.rawHeaderList()*/;
    foreach(QByteArray key, request.rawHeaderList()) {
        QLOG_DEBUG() << "\t" << key << request.rawHeader(key);
    }
}
