#ifndef VSC_BASICENTITY_H
#define VSC_BASICENTITY_H

#include <QtCore/QJsonObject>
#include <QtCore/QDateTime>
#include <QtCore/QObject>

class BasicEntity : public QObject
{
Q_OBJECT
Q_PROPERTY(QDateTime createdAt READ createdAt)
Q_PROPERTY(QString createdBy READ createdBy)

public:
    QDateTime startAt() const;
    QDateTime endAt() const;

    QDateTime createdAt() const;
    QString createdBy() const;

    QDateTime updateAt() const;
    QString updateBy() const;

protected:
    void readBasicAttributes(const QJsonObject &jsonObject);

    QDateTime m_startAt;
    QDateTime m_endAt;

    QDateTime m_createdAt;
    QString m_createdBy;

    QDateTime m_updatedAt;
    QString m_updatedBy;
};

#endif // VSC_BASICENTITY_H
