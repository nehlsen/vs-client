#include <QtCore/QJsonObject>
#include "LinkPicture.h"

#define LINK_PICTURE_URL DEV_MODE_PREFIX "/api/v1/venues/%1/pictures.json"

QNetworkRequest LinkPicture::createRequest()
{
    QNetworkRequest request(QUrl(server() + QString(LINK_PICTURE_URL).arg(requestParameters().at(0))));
    addJsonContentHeader(request);

    return request;
}

QJsonDocument LinkPicture::payload()
{
    QJsonObject field;
    field["uri"] = requestParameters().at(1);

    QJsonObject payload;
    payload["vesh_venue_picture"] = field;

    return QJsonDocument(payload);
}

bool LinkPicture::isMatch(const QUrl &requestUrl)
{
    return requestUrl.toString().contains(QString(LINK_PICTURE_URL).arg(requestParameters().at(0)));
}
