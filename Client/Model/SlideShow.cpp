#include "SlideShow.h"
#include <QtCore/QTimer>
#include <QsLog/QsLog.h>
#include <Client/Client.h>

SlideShow::SlideShow(QObject *parent) :
    QObject(parent),
    m_picturesCurrentIndex(-1)
{
    initTimer();
}

SlideShow::SlideShow(Client *client) :
    QObject(client),
    m_picturesCurrentIndex(-1)
{
    initTimer();
    connectClient(client);
}

void SlideShow::setInterval(int msec)
{
    m_delayAdvancePicture->start(qMax(msec, 1000));
}

int SlideShow::interval() const
{
    return m_delayAdvancePicture->interval();
}

VenuePicture* SlideShow::currentPicture() const
{
    if (m_picturesCurrentIndex >= 0) {
        return m_pictures.at(m_picturesCurrentIndex);
    }

    return new VenuePicture;
}

int SlideShow::currentPictureIndex() const
{
    return m_picturesCurrentIndex;
}

int SlideShow::countPictures() const
{
    return m_pictures.count();
}

void SlideShow::onPictureListCleared()
{
    m_pictures.clear();
    m_picturesCurrentIndex = -1;

    emit cleared();
}

void SlideShow::onPictureReady(VenuePicture *picture)
{
    m_pictures << picture;
}

void SlideShow::advancePicture()
{
    if (m_pictures.isEmpty()) {
        return;
    }

    if (m_picturesCurrentIndex < 0) {
        m_picturesCurrentIndex = 0;
    } else {
        m_picturesCurrentIndex++;
    }

    if (m_picturesCurrentIndex >= m_pictures.count()) {
        m_picturesCurrentIndex = 0;
    }

    QLOG_INFO() << "Slideshow::advancePicture(), index:" << m_picturesCurrentIndex;

    emit showPicture(m_pictures.at(m_picturesCurrentIndex));
}

void SlideShow::connectClient(Client *client)
{
    connect(client->venuePictures(), &VenuePictures::pictureListCleared,
            this, &SlideShow::onPictureListCleared);
    connect(client->venuePictures(), &VenuePictures::pictureReady,
            this, &SlideShow::onPictureReady);
}

void SlideShow::initTimer()
{
    m_delayAdvancePicture = new QTimer(this);
    m_delayAdvancePicture->setSingleShot(false);
    connect(m_delayAdvancePicture, &QTimer::timeout,
            this, &SlideShow::advancePicture);
}
