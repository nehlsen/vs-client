#include "Client.h"
#include <QsLog/QsLog.h>
#include <QtCore/QTimer>

Client::Client(QObject *parent):
    QObject(parent),
    m_autoRefreshEnabled(true)
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

void Client::requestFinished()
{
    QLOG_TRACE() << "Client::requestFinished()";

    auto *reply = dynamic_cast<QNetworkReply*>(sender());
    if (!reply) {
        return;
    }

    const int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QUrl &requestUrl = reply->request().url();

    QLOG_INFO() << requestUrl.toString()
                << httpStatusCode
                << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

    foreach(QByteArray key, reply->rawHeaderList()) {
        QLOG_DEBUG() << "\t" << key << reply->rawHeader(key);
    }

    const QByteArray responseBody = reply->readAll();
    QLOG_TRACE() << responseBody;

    if (m_endpointAcquireToken.isMatch(requestUrl)) {
        m_endpointAcquireToken.parseResponse(httpStatusCode, responseBody);
        setToken(m_endpointAcquireToken.token());
    } else if (m_endpointGetVenue.isMatch(requestUrl)) {
        m_endpointGetVenue.parseResponse(httpStatusCode, responseBody);
        setVenue(m_endpointGetVenue.venue());
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
