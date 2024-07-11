#include "KratosAuthenticate.h"

#define REQUEST_PARAMETERS_INDEX_ACTION_URL 2
#define REQUEST_PARAMETERS_INDEX_USERNAME 0
#define REQUEST_PARAMETERS_INDEX_PASSWORD 1

QNetworkRequest KratosAuthenticate::createRequest()
{
    QNetworkRequest request(QUrl(requestParameters().at(REQUEST_PARAMETERS_INDEX_ACTION_URL)));
    addJsonContentHeader(request);

    return request;
}

QJsonDocument KratosAuthenticate::payload()
{
    QJsonObject payload;
    payload["identifier"] = requestParameters().at(REQUEST_PARAMETERS_INDEX_USERNAME);
    payload["password"] = requestParameters().at(REQUEST_PARAMETERS_INDEX_PASSWORD);
    payload["method"] = "password";

    return QJsonDocument(payload);
}

KratosSessionToken KratosAuthenticate::token() const
{
    return m_token;
}

bool KratosAuthenticate::handleJsonDocument(const QJsonDocument &document)
{
    m_token = KratosSessionToken::fromJsonObject(document.object());

    return m_token.isValid();
}
