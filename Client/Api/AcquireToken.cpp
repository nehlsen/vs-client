#include <QsLog/QsLog.h>
#include "AcquireToken.h"

#define ACQUIRE_TOKEN_URL DEV_MODE_PREFIX "/api/login_check"

QNetworkRequest AcquireToken::createRequest()
{
    QNetworkRequest request(QUrl(server() + ACQUIRE_TOKEN_URL));
    addJsonContentHeader(request);

    return request;
}

QJsonDocument AcquireToken::payload()
{
    QJsonObject payload;
    payload["_username"] = requestParameters().at(0);
    payload["_password"] = requestParameters().at(1);

    return QJsonDocument(payload);
}

JwtToken AcquireToken::token() const
{
    return m_token;
}

bool AcquireToken::handleJsonDocument(const QJsonDocument &document)
{
    m_token = JwtToken::fromJsonObject(document.object());

    return m_token.isValid();
}
