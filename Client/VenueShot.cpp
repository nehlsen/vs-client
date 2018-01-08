#include <QtQml/QtQml>
#include <QsLog/QsLog.h>
#include <QtGui/QImage>
#include "VenueShot.h"

void VenueShot::declareQml()
{
    qmlRegisterType<VenueShot>("org.nehlsen.vs", 1, 0, "VenueShot");
    qmlRegisterType<Client>("org.nehlsen.vs.client", 1, 0, "ClientStatus");
    qmlRegisterType<VenuePicture>("org.nehlsen.vs.picture", 1, 0, "VenuePicture");
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

int VenueShot::autoUpdateInterval() const
{
    return m_client->venuePictures()->autoUpdateInterval();
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

void VenueShot::postPicture(const QImage &image)
{
    m_client->postPicture(image);
}

void VenueShot::postPicture(const QString &imageUri)
{
    QImage image;
    if (image.load(imageUri)) {
        postPicture(image);
    } else {
        QLOG_ERROR() << "VenueShot::postPicture() failed to load image from:" << imageUri;
    }
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

void VenueShot::setAutoUpdateInterval(int seconds)
{
    m_client->venuePictures()->setAutoUpdateInterval(seconds);
}

void VenueShot::setCacheFolder(const QString &folder)
{
    m_client->venuePictures()->setCacheFolder(folder);
}

void VenueShot::setInterval(int interval)
{
    m_slideShow->setInterval(interval);
}
