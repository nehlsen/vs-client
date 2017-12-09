#include "VenuePicture.h"
#include <QsLog/QsLog.h>

QString VenuePicture::uri() const
{
    return m_uri;
}

bool VenuePicture::isValid() const
{
    return !uri().isEmpty();
}

VenuePicture VenuePicture::fromJsonObject(const QJsonObject &jsonObject)
{
    QLOG_TRACE() << "VenuePicture::fromJsonObject()";

    VenuePicture picture;

    if (!jsonObject.contains(QLatin1String("uri"))) {
        QLOG_ERROR() << "missing uri element";
        return picture;
    }

    picture.m_uri = jsonObject.value(QLatin1String("uri")).toString();
    QLOG_TRACE() << "\t" << picture.m_uri;

    return picture;
}

void VenuePicture::setLocalPath(const QString &path)
{
    m_localPath = path;
}

QString VenuePicture::localPath() const
{
    return m_localPath;
}
