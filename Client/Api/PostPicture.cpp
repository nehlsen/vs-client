#include "PostPicture.h"
#include <QtNetwork/QHttpMultiPart>
#include <QtGui/QImage>
#include <QsLog/QsLog.h>
#include <QtNetwork/QNetworkReply>

#define POST_PICTURE_URL DEV_MODE_PREFIX "/api/v1/pictures.json"
#define POST_FORM_FIELD_NAME "vesh_picture[file][file]"

PostPicture::PostPicture() :
    m_payload(nullptr)
{
}

QNetworkRequest PostPicture::createRequest()
{
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

bool PostPicture::parseResponse(QNetworkReply *reply)
{
    m_publicLocation.clear();

    // using basic endpoint for initial parsing and debug output (but ignore return value)
    BasicEndpoint::parseResponse(reply);

    if (reply->hasRawHeader("X-Public-Location")) {
        m_publicLocation = QString::fromLatin1(reply->rawHeader("X-Public-Location"));
        return true;
    }

    return false;
}

bool PostPicture::parseResponse(int httpStatusCode, const QByteArray &responseBody)
{
    return false;
}

QString PostPicture::publicLocation() const
{
    return m_publicLocation;
}
