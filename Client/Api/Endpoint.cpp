#include "Endpoint.h"
#include <QsLog/QsLog.h>

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
    if (response.isEmpty()) {
        QLOG_WARN() << "Endpoint::parseResponse: Response is empty JSON";
        return false;
    }

    return handleJsonDocument(response);
}

void Endpoint::addJsonContentHeader(QNetworkRequest &request)
{
    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    request.setRawHeader("Content-Type", "application/json; charset=utf-8");
}
