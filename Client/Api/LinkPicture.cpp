#include <QtCore/QJsonObject>
#include <QsLog/QsLog.h>
#include "LinkPicture.h"

#define LINK_PICTURE_URL "%1/picture/"

QNetworkRequest LinkPicture::createRequest()
{
    QNetworkRequest request(QUrl(server() + QString(LINK_PICTURE_URL).arg(requestParameters().at(0))));
    addJsonContentHeader(request);

    return request;
}

QJsonDocument LinkPicture::payload()
{
    QJsonObject payload;
    payload["url"] = requestParameters().at(1);

    return QJsonDocument(payload);
}

bool LinkPicture::handleJsonDocument(const QJsonDocument &document)
{
    return false;
}
