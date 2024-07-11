#include "KratosSessionToken.h"
#include <QsLog/QsLog.h>

QString KratosSessionToken::token() const
{
    return m_token;
}

QDateTime KratosSessionToken::expiresAt() const
{
    return m_expiresAt;
}

bool KratosSessionToken::isValid() const
{
    return !token().isEmpty() && expiresAt().isValid() && !isLifetimeExpired();
}

bool KratosSessionToken::isLifetimeExpired() const
{
    return !expiresAt().isValid() || expiresAt().secsTo(QDateTime::currentDateTime()) < 1;
}

KratosSessionToken KratosSessionToken::fromJsonObject(const QJsonObject &jsonObject)
{
    QLOG_TRACE() << "KratosSessionToken::fromJsonObject()";

    KratosSessionToken token;
    
    if (!jsonObject.contains(QLatin1String("session_token"))) {
        QLOG_ERROR() << "KratosSessionToken::fromJsonObject(): missing session_token";
        return token;
    }
    
    if (!jsonObject.contains(QLatin1String("session"))) {
        QLOG_ERROR() << "KratosSessionToken::fromJsonObject(): missing session";
        return token;
    }
    const QJsonObject &session = jsonObject.value(QLatin1String("session")).toObject();
    if (!session.contains(QLatin1String("expires_at"))) {
        QLOG_ERROR() << "KratosSessionToken::fromJsonObject(): missing session.expires_at";
        return token;
    }
    
    token.m_token = jsonObject.value(QLatin1String("token")).toString();
    token.m_expiresAt = QDateTime::fromString(session.value(QLatin1String("expires_at")).toString());
    
    return token;
}
