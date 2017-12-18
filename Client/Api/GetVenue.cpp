#include <QsLog/QsLog.h>
#include "GetVenue.h"

#define GET_VENUE_URL DEV_MODE_PREFIX "/api/v1/venues/%1.json"

QNetworkRequest GetVenue::createRequest()
{
    QString query = QString(GET_VENUE_URL).arg(requestParameters().at(0));

    QNetworkRequest request(QUrl(server() + query));
    addJsonContentHeader(request);

    return request;
}

bool GetVenue::handleJsonDocument(const QJsonDocument &document)
{
    m_venue = Venue::fromJsonObject(document.object());

    return m_venue.isValid();
}

Venue GetVenue::venue() const
{
    return m_venue;
}
