#ifndef VSC_POSTPICTURE_H
#define VSC_POSTPICTURE_H

#include <QtCore/QBuffer>
#include "BasicEndpoint.h"

class QHttpMultiPart;

class PostPicture : public BasicEndpoint
{
public:
    PostPicture();

    QNetworkRequest createRequest() override;
    QHttpMultiPart *payload(const QImage &image);

    using BasicEndpoint::isMatch;
    bool isMatch(const QUrl &requestUrl) override;
    bool parseResponse(QNetworkReply *reply) override;
    bool parseResponse(int httpStatusCode, const QByteArray &responseBody) override;

    QString publicLocation() const;

protected:
    QByteArray m_bufferBytes;
    QBuffer m_bufferDevice;
    QHttpMultiPart *m_payload;
    QString m_publicLocation;
};

#endif // VSC_POSTPICTURE_H
