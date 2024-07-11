#ifndef VENUESHOTCLIENT_KRATOSSESSIONTOKEN_H
#define VENUESHOTCLIENT_KRATOSSESSIONTOKEN_H

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QJsonObject>

class KratosSessionToken
{
public:
    // .session_token
    QString token() const;

    // .session.expires_at
    QDateTime expiresAt() const;

    bool isValid() const;
    bool isLifetimeExpired() const;

    static KratosSessionToken fromJsonObject(const QJsonObject &jsonObject);

protected:
    QString m_token;
    QDateTime m_expiresAt;
};

#endif //VENUESHOTCLIENT_KRATOSSESSIONTOKEN_H
