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

protected:
    QString m_uri;
};

#endif // VSC_VENUEPICTURE_H
