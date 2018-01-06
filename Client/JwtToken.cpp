#include "JwtToken.h"
#include <QsLog/QsLog.h>
#include <QtCore/QJsonDocument>

JwtToken::JwtToken() :
    m_lifetime(-1)
{
}

QString JwtToken::token() const
{
    return m_token;
}

int JwtToken::lifetime() const
{
    return m_lifetime;
}

int JwtToken::lifetimeRemaining() const
{
    if (lifetime() <= 0) {
        return 0;
    }

    return lifetime() - m_created.secsTo(QDateTime::currentDateTime()) -1;
}

bool JwtToken::isValid() const
{
    return m_created.isValid() && !isLifetimeExpired();
}

bool JwtToken::isLifetimeExpired() const
{
    return lifetimeRemaining() <= 0;
}

JwtToken JwtToken::fromJsonObject(const QJsonObject &jsonObject)
{
    QLOG_TRACE() << "JwtToken::fromJsonObject()";

    JwtToken token;

    if (!jsonObject.contains(QLatin1String("token"))) {
        QLOG_ERROR() << "JwtToken::fromJsonObject(): missing token";
        return token;
    }

    token.m_created = QDateTime::currentDateTime();
    token.m_token = jsonObject.value(QLatin1String("token")).toString();
    token.m_lifetime = extractLifetime(token.m_token);

    QLOG_INFO() << "Token created - token will expire in" << token.lifetimeRemaining() << "seconds";

    return token;
}

int JwtToken::extractLifetime(const QString &token)
{
    QStringList parts = token.split('.'); // header, payload, signature
    if (parts.length() != 3) {
        return -1;
    }

    auto jsonDocument = QJsonDocument::fromJson(QByteArray::fromBase64(parts.at(1).toLatin1()));
    if (!jsonDocument.isObject()) {
        QLOG_ERROR() << "JwtToken::extractLifetime(): Failed to extract payload";
        return -1;
    }

    if (!jsonDocument.object().contains(QLatin1String("iat"))) {
        QLOG_ERROR() << "JwtToken::extractLifetime(): payload missing iat";
        return -1;
    }
    int issuedAt = jsonDocument.object().value(QLatin1String("iat")).toInt();

    if (!jsonDocument.object().contains(QLatin1String("exp"))) {
        QLOG_ERROR() << "JwtToken::extractLifetime(): payload missing exp";
        return -1;
    }
    int expireAt = jsonDocument.object().value(QLatin1String("exp")).toInt();

    return expireAt-issuedAt;

//    jsonDocument.object().value(QLatin1String("iat"));
//    jsonDocument.object().value(QLatin1String("exp"));
//    jsonDocument.object().value(QLatin1String("username"));
//    jsonDocument.object().value(QLatin1String("roles"));
}
