#ifndef VSC_POSTPICTURE_H
#define VSC_POSTPICTURE_H

#include <QtCore/QBuffer>
#include "Endpoint.h"

class QHttpMultiPart;

class PostPicture : public Endpoint
{
public:
    PostPicture();

    QNetworkRequest createRequest() override;
    QHttpMultiPart *payload(const QImage &image);

    QString publicLocation() const;

protected:
    bool handleJsonDocument(const QJsonDocument &document) override;

    QByteArray m_bufferBytes;
    QBuffer m_bufferDevice;
    QHttpMultiPart *m_payload;
    QString m_publicLocation;
};

#endif // VSC_POSTPICTURE_H
