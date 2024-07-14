#include <QtCore/QDateTime>
#include <QtCore/QJsonObject>
#include "BasicEntity.h"

QDateTime BasicEntity::startAt() const
{
    return m_startAt;
}

QDateTime BasicEntity::endAt() const
{
    return m_endAt;
}

QDateTime BasicEntity::createdAt() const
{
    return m_createdAt;
}

QString BasicEntity::createdBy() const
{
    return m_createdBy;
}

QDateTime BasicEntity::updateAt() const
{
    return m_updatedAt;
}

QString BasicEntity::updateBy() const
{
    return m_updatedBy;
}

void BasicEntity::readBasicAttributes(const QJsonObject &jsonObject)
{
    // FIXME where is it used?
    if (jsonObject.contains(QLatin1String("start_at"))) {
        m_startAt = QDateTime::fromString(jsonObject.value(QLatin1String("start_at")).toString(), Qt::ISODate);
    }
    if (jsonObject.contains(QLatin1String("end_at"))) {
        m_endAt = QDateTime::fromString(jsonObject.value(QLatin1String("end_at")).toString(), Qt::ISODate);
    }

    if (jsonObject.contains(QLatin1String("created_at"))) {
        m_createdAt = QDateTime::fromString(jsonObject.value(QLatin1String("created_at")).toString(), Qt::ISODate);
    }
    if (jsonObject.contains(QLatin1String("created_by"))) {
        m_createdBy = jsonObject.value(QLatin1String("created_by")).toString();
    }
    if (jsonObject.contains(QLatin1String("updated_at"))) {
        m_updatedAt = QDateTime::fromString(jsonObject.value(QLatin1String("updated_at")).toString(), Qt::ISODate);
    }
    if (jsonObject.contains(QLatin1String("updated_by"))) {
        m_updatedBy = jsonObject.value(QLatin1String("updated_by")).toString();
    }
}
