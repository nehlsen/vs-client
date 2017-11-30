#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QsLog/QsLog.h>
#include <QtCore/QJsonDocument>
#include <QtCore/QTimer>
#include <QtCore/QJsonObject>
#include "Client.h"

#define DEV_MODE_PREFIX "/app_dev.php"

namespace Url
{
const QLatin1String AcquireToken(DEV_MODE_PREFIX "/api/login_check");
const QLatin1String RelativeApiBase(DEV_MODE_PREFIX "/api/v1");
const QLatin1String GetPictures("/pictures");
const QLatin1String GetPicture("/picture/ID");
const QLatin1String PostPicture("/pictures");
}

Client::Client(QObject *parent):
    QObject(parent),
    m_autoRefreshEnabled(true),
    m_token(-1)
{
    QLOG_TRACE() << "Client::Client";
}

void Client::setServer(const QUrl &server)
{
    m_server = server;
}

QUrl Client::server() const
{
    return m_server;
}

void Client::setUsername(const QString &username)
{
    m_username = username;
}

QString Client::username() const
{
    return m_username;
}

void Client::setPassword(const QString &password)
{
    m_password = password;
}

QString Client::password() const
{
    return m_password;
}

void Client::setTokenAutoRefreshEnabled(bool enabled)
{
    m_autoRefreshEnabled = enabled;
}

bool Client::isTokenAutoRefreshEnabled() const
{
    return m_autoRefreshEnabled;
}

JwtToken Client::token() const
{
    return m_token;
}

void Client::acquireToken()
{
    acquireToken(username(), password());
}

void Client::acquireToken(const QString &username, const QString &password)
{
    QLOG_TRACE() << "Client::acquireToken";

    QJsonObject payload;
    payload["_username"] = username;
    payload["_password"] = password;

    QNetworkRequest request = QNetworkRequest(createUrl(Url::AcquireToken));
    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    request.setRawHeader("Content-Type", "application/json; charset=utf-8");

    post(request, payload);
}

void Client::requestFinished()
{
    QLOG_TRACE() << "Client::requestFinished()";

    auto *reply = dynamic_cast<QNetworkReply*>(sender());
    if (!reply) {
        return;
    }

    const int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    QLOG_INFO() << reply->request().url().toString()
                << httpStatusCode
                << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

    foreach(QByteArray key, reply->rawHeaderList()) {
        QLOG_DEBUG() << "\t" << key << reply->rawHeader(key);
    }

    const QByteArray responseBody = reply->readAll();
    const QString path = extractPath(reply->request().url());

    if (path == Url::AcquireToken) {
        handleAcquireTokenResponse(httpStatusCode, responseBody);
    } /*else if (path == Url::Test) {
        handleTestResponse(httpStatusCode, responseBody);
    }*/ else {
        QLOG_TRACE() << responseBody;
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

void Client::setToken(JwtToken token)
{
    m_token = token;
    emit tokenChanged(m_token);
}

QUrl Client::createUrl(const QString &path) const
{
    // https://stage1.miofair.de/oauth/v2/token
    // https://stage1.miofair.de/api/v2/xxx

    QString basePath;
    if (!path.startsWith('/')) {
        // relative path, prepend "/api/v2/"
        basePath = m_server.path();
        if (!basePath.endsWith('/')) {
            basePath += '/';
        }
        basePath += Url::RelativeApiBase;
    }
    // else: absolute path, use only provided part

    QUrl url(m_server);
    url.setPath(basePath + path);
    return url;
}

QString Client::extractPath(const QUrl &url) const
{
    const QString absoluteApiBase = QString("/%1").arg(Url::RelativeApiBase);

    QString path = url.path();
    if (path.startsWith(absoluteApiBase)) {
        path = path.mid(absoluteApiBase.size());
    }

    return path;
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

    if (token().isLifetimeExpired()) {
        QLOG_ERROR() << "need a valid token to do requests";
        return nullptr;
    }

    QNetworkRequest req = prepareRequest(request);
    dumpRequestInfo("POST", req);
    return prepareReply(m_qnam.post(req, payload));
}

QNetworkReply *Client::get(QNetworkRequest request)
{
    QLOG_TRACE() << "Client::get()" << request.url();

    if (token().isLifetimeExpired()) {
        QLOG_ERROR() << "need a valid token to do requests";
        return nullptr;
    }

    QNetworkRequest req = prepareRequest(request);
    dumpRequestInfo("GET", req);
    return prepareReply(m_qnam.get(req));
}

QNetworkRequest Client::prepareRequest(QNetworkRequest request)
{
    if (!request.hasRawHeader("Authorization")) {
        request.setRawHeader("Authorization", QString("Bearer %1").arg(m_token.token()).toUtf8());
    }
    if (!request.hasRawHeader("Accept")) {
        request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    }
    if (!request.hasRawHeader("Content-Type")) {
        request.setRawHeader("Content-Type", "application/json; charset=utf-8");
    }

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

void Client::handleAcquireTokenResponse(const int httpStatusCode, const QByteArray &responseBody)
{
    QLOG_TRACE() << "Client::handleAcquireTokenResponse()" << httpStatusCode << responseBody;

    if (httpStatusCode != 200) {
        QLOG_ERROR() << "HTTP-Status not 200-OK";
        setToken(JwtToken(0));
        emit acquireTokenFailed();
        return;
    }

    QJsonDocument response = QJsonDocument::fromJson(responseBody);
    if (!response.isObject()) {
        QLOG_ERROR() << "Response is not a JSON Object";
        setToken(JwtToken(0));
        emit acquireTokenFailed();
        return;
    }

    setToken(JwtToken::fromJsonObject(response.object()));
    emit acquireTokenSucceed();
}
