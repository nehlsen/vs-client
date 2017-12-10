#ifndef VSC_VENUE_H
#define VSC_VENUE_H

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QDateTime>
#include "BasicEntity.h"

class Venue : public BasicEntity
{
public:
    QString token() const;
    QString name() const;
    QString description() const;

    bool isValid() const;

    static Venue fromJsonObject(const QJsonObject &jsonObject);

protected:
    QString m_token;
    QString m_name;
    QString m_description;
};

#endif // VSC_VENUE_H
