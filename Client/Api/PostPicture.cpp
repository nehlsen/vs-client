#include "PostPicture.h"
#include <QtNetwork/QHttpMultiPart>
#include <QtGui/QImage>
#include <QsLog/QsLog.h>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QJsonObject>

#define POST_PICTURE_URL ""
#define POST_FORM_FIELD_NAME "file"

PostPicture::PostPicture() :
    m_payload(nullptr)
{
}

QNetworkRequest PostPicture::createRequest()
{
    // FIXME replace server with pictureServiceServer - like with service and auth
    QNetworkRequest request(QUrl(server() + POST_PICTURE_URL));
    // FIXME add content header?

    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
//    request.setRawHeader("Content-Type", "multipart/form-data");

    return request;
}

QHttpMultiPart *PostPicture::payload(const QImage &image)
{
    if (nullptr != m_payload) {
        m_payload->deleteLater();
    }

    m_payload = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" POST_FORM_FIELD_NAME "\"; filename=\"upload.png\""));

    m_bufferDevice.close();
    m_bufferBytes.clear();
    m_bufferDevice.setData(m_bufferBytes);
    m_bufferDevice.open(QIODevice::WriteOnly);
    if (image.save(&m_bufferDevice, "PNG")) {
        QLOG_DEBUG() << "PostPicture::payload: image written to device";
    } else {
        QLOG_DEBUG() << "PostPicture::payload: failed to write image to device";
    }

    m_bufferDevice.close();
    m_bufferDevice.open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(&m_bufferDevice);
    m_payload->append(imagePart);

    return m_payload;
}

bool PostPicture::handleJsonDocument(const QJsonDocument &document)
{
    m_publicLocation.clear();

    const QJsonObject &jsonObject = document.object();
    
    if (!jsonObject.contains(QLatin1String("id"))) {
        QLOG_ERROR() << "PostPicture::handleJsonDocument(): missing id";
        return false;
    }

    m_publicLocation = server() + POST_PICTURE_URL + jsonObject.value("id").toString();

    return true;
}

QString PostPicture::publicLocation() const
{
    return m_publicLocation;
}
