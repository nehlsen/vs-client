#ifndef VSC_JWTTOKEN_H
#define VSC_JWTTOKEN_H

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QJsonObject>

class JwtToken
{
public:
    JwtToken();
    explicit JwtToken(int lifetime);

    QString token() const;

    int lifetime() const;
    int lifetimeRemaining() const;

    bool isValid() const;
    bool isLifetimeExpired() const;

    static JwtToken fromJsonObject(const QJsonObject &jsonObject, int lifetime = 60*60);

protected:
    QDateTime m_created;

    QString m_token;

    int m_lifetime;
};

#endif // VSC_JWTTOKEN_H
