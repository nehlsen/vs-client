#ifndef VSC_VENUEPICTURE_H
#define VSC_VENUEPICTURE_H

// currently, this contains only the picture URI
//   later it will contain createdAt/By, Rating, comments....

#include <QtCore/QString>
#include <QtCore/QJsonObject>

class VenuePicture
{
public:
    QString uri() const;

    bool isValid() const;

    static VenuePicture fromJsonObject(const QJsonObject &jsonObject);

    void setLocalPath(const QString &path);
    QString localPath() const;

protected:
    QString m_uri;
    QString m_localPath;
};

#endif // VSC_VENUEPICTURE_H
