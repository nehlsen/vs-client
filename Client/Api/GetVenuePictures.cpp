#include <QsLog/QsLog.h>
#include <QtCore/QJsonArray>
#include "GetVenuePictures.h"

#define GET_VENUE_PICTURES_URL "%1/picture/"

QNetworkRequest GetVenuePictures::createRequest()
{
    QString query = QString(GET_VENUE_PICTURES_URL).arg(requestParameters().at(0));

    if (requestParameters().size() > 1) {
        query += "?created-after=" + requestParameters().at(1);
    }

    QNetworkRequest request(QUrl(server() + query));
    addJsonContentHeader(request);

    return request;
}

bool GetVenuePictures::handleJsonDocument(const QJsonDocument &document)
{
    m_venuePictures.clear();

    if (!document.isArray()) {
        QLOG_WARN() << "GetVenuePictures::handleJsonDocument: expected JSON Array";
        return false;
    }

    if (document.array().isEmpty()) {
        QLOG_WARN() << "GetVenuePictures::handleJsonDocument: found empty JSON Array";
        return false;
    }

    for (int i = 0; i < document.array().size(); ++i) {
        auto picture = VenuePicture::fromJsonObject(document.array().at(i).toObject());
        if (picture->isValid()) {
            m_venuePictures << picture;
        }
    }

    return true;
}

QList<VenuePicture*> GetVenuePictures::venuePictures()
{
    return m_venuePictures;
}
