#include <QtQml/QtQml>
#include "VenueShot.h"

void VenueShot::declareQml()
{
    qmlRegisterType<VenueShot>("org.nehlsen.vs", 1, 0, "VenueShot");
    qmlRegisterType<Client>("org.nehlsen.vs.client", 1, 0, "ClientStatus");
}

VenueShot::VenueShot(QObject *parent) :
    QObject(parent),
    m_client(new Client(this)),
    m_slideShow(new SlideShow(m_client))
{
    connect(m_client, &Client::statusChanged, this, &VenueShot::statusChanged);
    connect(m_slideShow, &SlideShow::showPicture, this, &VenueShot::currentPictureChanged);
}

QString VenueShot::server() const
{
    return m_client->server();
}

bool VenueShot::isTokenAutoRefreshEnabled() const
{
    return m_client->isTokenAutoRefreshEnabled();
}

Client::Status VenueShot::status() const
{
    return m_client->status();
}

QString VenueShot::venueToken() const
{
    return m_client->venue()->token();
}

Venue *VenueShot::venue() const
{
    return m_client->venue();
}

bool VenueShot::isAutoFetchPicturesEnabled() const
{
    return m_client->venuePictures()->isAutoFetchPicturesEnabled();
}

bool VenueShot::isAutoUpdateEnabled() const
{
    return m_client->venuePictures()->isAutoUpdateEnabled();
}

QString VenueShot::cacheFolder() const
{
    return m_client->venuePictures()->cacheFolder();
}

int VenueShot::interval() const
{
    return m_slideShow->interval();
}

VenuePicture *VenueShot::currentPicture() const
{
//    return new VenuePicture; // FIXME
    return m_slideShow->currentPicture();
}

void VenueShot::setServer(const QString &uri)
{
    m_client->setServer(uri);
}

void VenueShot::setTokenAutoRefreshEnabled(bool enabled)
{
    m_client->setTokenAutoRefreshEnabled(enabled);
}

void VenueShot::acquireToken(const QString &username, const QString &password)
{
    m_client->acquireToken(username, password);
}

void VenueShot::setVenueToken(const QString &token)
{
    m_client->setVenue(token);
}

void VenueShot::setVenue(Venue *venue)
{
    // nope!
}

void VenueShot::setAutoFetchPicturesEnabled(bool enabled)
{
    m_client->venuePictures()->setAutoFetchPicturesEnabled(enabled);
}

void VenueShot::setAutoUpdateEnabled(bool enabled)
{
//    m_client->venuePictures()->setAutoUpdateInterval()
}

void VenueShot::setCacheFolder(const QString &folder)
{
    m_client->venuePictures()->setCacheFolder(folder);
}

void VenueShot::setInterval(int interval)
{
    m_slideShow->setInterval(interval);
}
