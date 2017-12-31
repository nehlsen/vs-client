#ifndef VSC_VENUEPICTURE_H
#define VSC_VENUEPICTURE_H

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QMetaType>
#include "BasicEntity.h"

// currently, this contains only the picture URI
//   later it will contain createdAt/By, Rating, comments....
class VenuePicture : public BasicEntity
{
Q_OBJECT
Q_PROPERTY(QString hash READ hash)
Q_PROPERTY(QString uri READ uri)
Q_PROPERTY(bool valid READ isValid)
Q_PROPERTY(QString localPath READ localPath)

public:
    // get something uniquely identifying this picture
    QString hash() const;

    QString uri() const;

    bool isValid() const;

    static VenuePicture* fromJsonObject(const QJsonObject &jsonObject);

    void setLocalPath(const QString &path);
    QString localPath() const;

protected:
    QString m_uri;
    QString m_localPath;
};

#endif // VSC_VENUEPICTURE_H
