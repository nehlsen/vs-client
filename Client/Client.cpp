#include "Client.h"
#include <QsLog/QsLog.h>
#include <QtCore/QTimer>
#include <QtNetwork/QHttpMultiPart>
#include <Client/Model/VenuePictures.h>

Client::Client(QObject *parent):
    QObject(parent),
    m_autoRefreshEnabled(true),
    m_venue(new Venue),
    m_venuePictures(new VenuePictures(this))
{
    QLOG_TRACE() << "Client::Client";
}

void Client::setServerService(const QString &server)
{
    m_serverService = server;
}

QString Client::serverService() const
{
    return m_serverService;
}

void Client::setServerStorage(const QString &server)
{
    m_serverStorage= server;
}

QString Client::serverStorage() const
{
    return m_serverStorage;
}

void Client::setTokenAutoRefreshEnabled(bool enabled)
{
    m_autoRefreshEnabled = enabled;
}

bool Client::isTokenAutoRefreshEnabled() const
{
    return m_autoRefreshEnabled;
}

Client::Status Client::status() const
{
    if (!token().isValid()) {
        return Offline;
    }
    if (!venue()->isValid()) {
        return Online;
    }

    return Ready;
}

JwtToken Client::token() const
{
    return m_token;
}

void Client::setVenue(const QString &venueToken)
{
    QLOG_TRACE() << "Client::setVenue(" << venueToken << ")";

    m_endpointGetVenue.setRequestParameters(serverService(), QStringList() << venueToken);
    QNetworkRequest request = m_endpointGetVenue.createRequest();
    request.setOriginatingObject(&m_endpointGetVenue);
    get(addAuthHeader(request));
}

Venue* Client::venue() const
{
    return m_venue;
}

VenuePictures *Client::venuePictures()
{
    return m_venuePictures;
}

void Client::acquireToken(const QString &username, const QString &password)
{
    QLOG_TRACE() << "Client::acquireToken";

    m_endpointAcquireToken.setRequestParameters(serverService(), QStringList() << username << password);
    QNetworkRequest request = m_endpointAcquireToken.createRequest();
    request.setOriginatingObject(&m_endpointAcquireToken);
    post(request, m_endpointAcquireToken.payload());
}

void Client::postPicture(const QImage &image)
{
    QLOG_TRACE() << "Client::postPicture";

    m_endpointPostPicture.setRequestParameters(serverStorage(), QStringList());
    QNetworkRequest request = m_endpointPostPicture.createRequest();
    request.setOriginatingObject(&m_endpointPostPicture);
    post(addAuthHeader(request), m_endpointPostPicture.payload(image));
}

void Client::linkPicture(const QString &publicPictureLocation)
{
    QLOG_TRACE() << "Client::linkPicture(" << publicPictureLocation << ")";

    QStringList parameters;
    parameters << venue()->token()
               << publicPictureLocation;

    m_endpointLinkPicture.setRequestParameters(serverService(), parameters);
    QNetworkRequest request = m_endpointLinkPicture.createRequest();
    request.setOriginatingObject(&m_endpointLinkPicture);
    post(addAuthHeader(request), m_endpointLinkPicture.payload());
}

void Client::getVenuePictures()
{
    QLOG_TRACE() << "Client::getVenuePictures()";
    if (!venue()->isValid()) {
        QLOG_ERROR() << "can not fetch pictures without a valid venue!";
        return;
    }

    m_endpointGetVenuePictures.setRequestParameters(serverService(), QStringList() << venue()->token());
    QNetworkRequest request = m_endpointGetVenuePictures.createRequest();
    request.setOriginatingObject(&m_endpointGetVenuePictures);
    get(addAuthHeader(request));
}

void Client::getVenuePictures(const QDateTime &createdAfter)
{
    QLOG_TRACE() << "Client::getVenuePictures(createdAfter:" << createdAfter << ")";
    if (!venue()->isValid()) {
        QLOG_ERROR() << "can not fetch pictures without a valid venue!";
        return;
    }

    QStringList parameters;
    parameters << venue()->token();
    parameters << createdAfter.toString("yyyy-MM-ddTHH:mm:ss");
    m_endpointGetVenuePictures.setRequestParameters(serverService(), parameters);
    QNetworkRequest request = m_endpointGetVenuePictures.createRequest();
    request.setOriginatingObject(&m_endpointGetVenuePictures);
    get(addAuthHeader(request));
}

void Client::requestFinished()
{
    QLOG_TRACE() << "Client::requestFinished()";

    auto *reply = dynamic_cast<QNetworkReply*>(sender());
    if (!reply) {
        return;
    }

    auto *origin = reply->request().originatingObject();
    if (origin == dynamic_cast<QObject*>(&m_endpointAcquireToken)) {
        m_endpointAcquireToken.parseResponse(reply);
        setToken(m_endpointAcquireToken.token());
    } else if (origin == dynamic_cast<QObject*>(&m_endpointGetVenue)) {
        m_endpointGetVenue.parseResponse(reply);
        setVenue(m_endpointGetVenue.venue());
    } else if (origin == dynamic_cast<QObject*>(&m_endpointPostPicture)) {
        m_endpointPostPicture.parseResponse(reply);
        emit pictureUploaded(m_endpointPostPicture.publicLocation());
        linkPicture(m_endpointPostPicture.publicLocation());
    } else if (origin == dynamic_cast<QObject*>(&m_endpointLinkPicture)) {
        m_endpointLinkPicture.parseResponse(reply);
        emit pictureLinked();
    } else if (origin == dynamic_cast<QObject*>(&m_endpointGetVenuePictures)) {
        m_endpointGetVenuePictures.parseResponse(reply);
        m_venuePictures->readUpdate(m_endpointGetVenuePictures.venuePictures());
    } else {
        QLOG_TRACE() << "Client::requestFinished(), failed to select appropriate endpoint to handle response!";
    }
}

void Client::requestError(QNetworkReply::NetworkError code)
{
    QLOG_TRACE() << "Client::requestError()" << code;

    if (code == QNetworkReply::AuthenticationRequiredError && !token().isValid()) {
        QLOG_INFO() << "Client::requestError(): Token expired";
        updateStatus();
    }
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

void Client::setVenue(Venue *venue)
{
    m_venue = venue;
    if (venue->isValid()) {
        emit venueChanged(m_venue);
    }

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
        QLOG_ERROR() << "Client::addAuthHeader(): Token is expired";
        return request;
    }
    if (!token().isValid()) {
        QLOG_ERROR() << "Client::addAuthHeader(): Token is NOT valid";
        return request;
    }

    request.setRawHeader("Authorization", QString("Bearer %1").arg(token().token()).toUtf8());

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
