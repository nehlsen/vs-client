#include "Venue.h"
#include <QsLog/QsLog.h>

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

    if (!jsonObject.contains(QLatin1String("venue"))) {
        QLOG_ERROR() << "missing venue root element";
        return venue;
    }
    const QJsonObject &jsonRoot = jsonObject.value(QLatin1String("venue")).toObject();

    if (!jsonRoot.contains(QLatin1String("token"))) {
        QLOG_ERROR() << "missing token";
        return venue;
    }
    if (!jsonRoot.contains(QLatin1String("name"))) {
        QLOG_ERROR() << "missing name";
        return venue;
    }

    venue->m_token = jsonRoot.value(QLatin1String("token")).toString();
    venue->m_name = jsonRoot.value(QLatin1String("name")).toString();

    if (jsonRoot.contains(QLatin1String("description"))) {
        venue->m_description = jsonRoot.value(QLatin1String("description")).toString();
    }

    venue->readBasicAttributes(jsonRoot);

    return venue;
}
