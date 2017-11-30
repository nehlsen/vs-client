#include <QsLog/QsLog.h>
#include "JwtToken.h"

JwtToken::JwtToken(int lifetime):
    m_lifetime(lifetime)
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
    if (!isValid()) {
        return -1;
    }

    return lifetime() - m_created.secsTo(QDateTime::currentDateTime()) -1;
}

bool JwtToken::isValid() const
{
    return m_created.isValid() && m_lifetime > 0;
}

bool JwtToken::isLifetimeExpired() const
{
    return lifetimeRemaining() <= 0;
}

JwtToken JwtToken::fromJsonObject(const QJsonObject &jsonObject, int lifetime)
{
    QLOG_TRACE() << "JwtToken::fromJsonObject()";

     JwtToken token(lifetime);

    if (!jsonObject.contains(QLatin1String("token"))) {
        QLOG_ERROR() << "missing token";
        return token;
    }

    token.m_created = QDateTime::currentDateTime();
    token.m_token = jsonObject.value(QLatin1String("token")).toString();

    QLOG_INFO() << "Token created - token will expire in" << token.lifetimeRemaining() << "seconds";

    return token;
}
