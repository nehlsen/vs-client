#include "Endpoint.h"
#include <QsLog/QsLog.h>

void Endpoint::setRequestParameters(const QString &server, const QStringList &parameters)
{
    m_server = server;
    m_requestParameters = parameters;
}

QString Endpoint::server() const
{
    return m_server;
}

QStringList Endpoint::requestParameters() const
{
    return m_requestParameters;
}

QJsonDocument Endpoint::payload()
{
    return QJsonDocument();
}

bool Endpoint::parseResponse(int httpStatusCode, const QByteArray &responseBody)
{
    if (httpStatusCode < 200 || httpStatusCode >= 300) {
        QLOG_ERROR() << "Endpoint::parseResponse: HTTP-Status not 2xx";
        return false;
    }

    QJsonDocument response = QJsonDocument::fromJson(responseBody);
    if (!response.isObject()) {
        QLOG_ERROR() << "Endpoint::parseResponse: Response is not a JSON Object";
        return false;
    }

    return handleJsonDocument(response);
}

bool Endpoint::handleJsonDocument(const QJsonDocument &document)
{
    return false;
}

void Endpoint::addContentHeader(QNetworkRequest &request)
{
    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    request.setRawHeader("Content-Type", "application/json; charset=utf-8");
}
