#ifndef VSC_VENUE_H
#define VSC_VENUE_H

#include <QtCore/QString>
#include <QtCore/QJsonObject>

class Venue
{
public:
    int id() const;
    QString token() const;
    QString name() const;
    QString description() const;

    bool isValid() const;

    static Venue fromJsonObject(const QJsonObject &jsonObject);

protected:
    int m_id;
    QString m_token;
    QString m_name;
    QString m_description;
};

#endif // VSC_VENUE_H
