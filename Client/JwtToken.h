#ifndef VSC_JWTTOKEN_H
#define VSC_JWTTOKEN_H

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QJsonObject>

class JwtToken
{
public:
    JwtToken();

    QString token() const;

    int lifetime() const;
    int lifetimeRemaining() const;

    bool isValid() const;
    bool isLifetimeExpired() const;

    static JwtToken fromJsonObject(const QJsonObject &jsonObject);

protected:
    QDateTime m_created;
    QString m_token;
    int m_lifetime;

    static int extractLifetime(const QString &token);
};

#endif // VSC_JWTTOKEN_H
