#include "GetVenue.h"

#define GET_VENUE_URL DEV_MODE_PREFIX "/api/v1/venues/%1.json"

QNetworkRequest GetVenue::createRequest()
{
    QString query = QString(GET_VENUE_URL).arg(requestParameters().at(0));

    QNetworkRequest request = QNetworkRequest(QUrl(server() + query));
    addContentHeader(request);

    return request;
}

bool GetVenue::handleJsonDocument(const QJsonDocument &document)
{
    m_venue = Venue::fromJsonObject(document.object());

    return m_venue.isValid();
}

bool GetVenue::isMatch(const QUrl &requestUrl)
{
    const QString &url = QString(GET_VENUE_URL);
    return requestUrl.toString().contains(url.left(url.size()-7));
}
