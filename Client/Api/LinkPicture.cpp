#include <QtCore/QJsonObject>
#include <QsLog/QsLog.h>
#include "LinkPicture.h"

#define LINK_PICTURE_URL DEV_MODE_PREFIX "/v1/venues/%1/pictures.json"

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
    payload["picture"] = field;

    return QJsonDocument(payload);
}

bool LinkPicture::handleJsonDocument(const QJsonDocument &document)
{
    return false;
}
