#include "VenuePictures.h"
#include <Client/Client.h>
#include <QsLog/QsLog.h>

VenuePictures::VenuePictures(Client *client):
    m_client(client),
    m_autoFetchPicturesEnabled(false)
{
    connect(m_client, &Client::venueChanged,
            this, &VenuePictures::onVenueChanged);
}

void VenuePictures::setAutoFetchPicturesEnabled(bool enabled)
{
    m_autoFetchPicturesEnabled = enabled;
}

bool VenuePictures::isAutoFetchPicturesEnabled() const
{
    return m_autoFetchPicturesEnabled;
}

void VenuePictures::update()
{
    m_client->getVenuePictures();
}

void VenuePictures::readUpdate(const QList<VenuePicture> &pictureList)
{
    // for now just replace
    //  later on try to calculate diffs and issue signals appropriately
    m_pictureList = pictureList;
    emit picturesAdded(m_pictureList);

    QLOG_INFO() << "VenuePictures::readUpdate, new picture count:" << m_pictureList.size();
}

void VenuePictures::onVenueChanged(const Venue &venue)
{
    clearPictureList();
    if (isAutoFetchPicturesEnabled()) {
        update();
    }
}

void VenuePictures::clearPictureList()
{
    m_pictureList.clear();
    emit pictureListCleared();
}
