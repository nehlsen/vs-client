#include <QtCore/QJsonObject>
#include "LinkPicture.h"

#define LINK_PICTURE_URL DEV_MODE_PREFIX "/api/v1/venues/%1/picture.json"

QNetworkRequest LinkPicture::createRequest()
{
    QNetworkRequest request(QUrl(server() + QString(LINK_PICTURE_URL).arg(requestParameters().at(0))));
    addJsonContentHeader(request);

    return request;
}

QJsonDocument LinkPicture::payload()
{
    QJsonObject payload;
    payload["uri"] = requestParameters().at(1);

    return QJsonDocument(payload);
}

bool LinkPicture::isMatch(const QUrl &requestUrl)
{
    return requestUrl.toString().contains(QString(LINK_PICTURE_URL).arg(requestParameters().at(0)));
}
