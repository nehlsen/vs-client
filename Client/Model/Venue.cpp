#include "Venue.h"
#include <QsLog/QsLog.h>

QString Venue::id() const
{
    return m_id;
}

QString Venue::token() const
{
    return m_token;
}

QString Venue::name() const
{
    return m_name;
}

QString Venue::description() const
{
    return m_description;
}

bool Venue::isValid() const
{
    return !token().isEmpty() && !name().isEmpty();
}

Venue* Venue::fromJsonObject(const QJsonObject &jsonObject)
{
    QLOG_TRACE() << "Venue::fromJsonObject()";

    auto venue = new Venue;

    if (!jsonObject.contains(QLatin1String("id"))) {
        QLOG_ERROR() << "Venue::fromJsonObject(): missing id";
        return venue;
    }
    if (!jsonObject.contains(QLatin1String("token"))) {
        QLOG_ERROR() << "Venue::fromJsonObject(): missing token";
        return venue;
    }
    if (!jsonObject.contains(QLatin1String("name"))) {
        QLOG_ERROR() << "Venue::fromJsonObject(): missing name";
        return venue;
    }

    venue->m_id = jsonObject.value(QLatin1String("id")).toString();
    venue->m_token = jsonObject.value(QLatin1String("token")).toString();
    venue->m_name = jsonObject.value(QLatin1String("name")).toString();

    if (jsonObject.contains(QLatin1String("description"))) {
        venue->m_description = jsonObject.value(QLatin1String("description")).toString();
    }

    venue->readBasicAttributes(jsonObject);

    return venue;
}
