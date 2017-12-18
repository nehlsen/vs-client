#include "VenuePictures.h"
#include <Client/Client.h>
#include <QsLog/QsLog.h>
#include <QtCore/QTimer>

VenuePictures::VenuePictures(Client *client):
    m_client(client),
    m_autoFetchPicturesEnabled(false),
    m_hasActiveDownload(false),
    m_updateIsRunning(false)
{
    connect(m_client, &Client::venueChanged,
            this, &VenuePictures::onVenueChanged);

    connect(&m_nam, &QNetworkAccessManager::finished,
            this, &VenuePictures::onDownloadFinished);

    m_autoUpdate = new QTimer(this);
    m_autoUpdate->setSingleShot(false);
    connect(m_autoUpdate, &QTimer::timeout,
            this, &VenuePictures::onAutoUpdateTimeout);
}

void VenuePictures::setAutoFetchPicturesEnabled(bool enabled)
{
    m_autoFetchPicturesEnabled = enabled;
}

bool VenuePictures::isAutoFetchPicturesEnabled() const
{
    return m_autoFetchPicturesEnabled;
}

void VenuePictures::setAutoUpdateInterval(int seconds)
{
    if (seconds >= 1) {
        m_autoUpdate->start(seconds * 1000);
        QLOG_INFO() << "VenuePictures::setAutoUpdateInterval(seconds:" << seconds << ")";
    } else {
        m_autoUpdate->stop();
        QLOG_INFO() << "VenuePictures::setAutoUpdateInterval() DISABLED";
    }
}

int VenuePictures::autoUpdateInterval() const
{
    return m_autoUpdate->interval();
}

bool VenuePictures::isAutoUpdateEnabled() const
{
    return m_autoUpdate->isActive();
}

void VenuePictures::setCacheFolder(const QString &folder)
{
    m_cacheFolder.setPath(folder);

    if (!m_cacheFolder.exists()) {
        m_cacheFolder.mkpath(folder);
    }

    Q_ASSERT(m_cacheFolder.exists());

    QLOG_INFO() << "VenuePictures::setCacheFolder(" << folder << "), saving cache files to: " << m_cacheFolder.absolutePath();
}

void VenuePictures::update()
{
    if (!m_client->venue().isValid()) {
        QLOG_WARN() << "VenuePictures::update(), no venue selected, not starting update";
        return;
    }

    if (m_updateIsRunning) {
        QLOG_ERROR() << "VenuePictures::update(), update running, not starting new one";
        return;
    }
    m_updateIsRunning = true;
    
    if (m_latestPictureCreated.isValid()) {
        m_client->getVenuePictures(m_latestPictureCreated);
    } else {
        m_client->getVenuePictures();
    }
}

void VenuePictures::readUpdate(const QList<VenuePicture> &pictureList)
{
    QLOG_INFO() << "VenuePictures::readUpdate, count:" << pictureList.size();

    for (const VenuePicture &vp : pictureList) {
        if (hasPicture(vp)) {
            // we already now this one, skip it
            continue;
        }

        if (isCached(vp)) {
            setPictureReady(vp);
        } else {
            addToDownloadQueue(vp);
        }
    }

    // FIXME possible race condition?
    //  100 pictures added to DL queue, update-running marked as false
    //  new update started (because update-running is false)
    //   99 of the 100 are added again
    //  queue grows indefinitely?
    m_updateIsRunning = false;
}

void VenuePictures::onVenueChanged(const Venue &venue)
{
    clearPictureList();
    if (isAutoFetchPicturesEnabled()) {
        update();
    }
}

void VenuePictures::onDownloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        QLOG_WARN() << "VenuePictures::onDownloadFinished, download failed:" << url.toString() << "\n"
                    << reply->errorString();

        m_downloadQueue.dequeue();
        reply->deleteLater();
        m_hasActiveDownload = false;
        QTimer::singleShot(100, this, &VenuePictures::startNextDownload);
        return;
    }

    if (isHttpRedirect(reply)) {
        QLOG_WARN() << "VenuePictures::onDownloadFinished, request was redirected, bailing!";

        m_downloadQueue.dequeue();
        reply->deleteLater();
        m_hasActiveDownload = false;
        QTimer::singleShot(100, this, &VenuePictures::startNextDownload);
        return;
    }

    QString basename = QFileInfo(url.path()).fileName();
    if (basename.isEmpty()) {
        QLOG_WARN() << "VenuePictures::onDownloadFinished, can not guess a filename for" << url << "bailing!";

        m_downloadQueue.dequeue();
        reply->deleteLater();
        m_hasActiveDownload = false;
        QTimer::singleShot(100, this, &VenuePictures::startNextDownload);
        return;
    }

    if (saveToDisk(basename, reply)) {
        setPictureReady(m_downloadQueue.dequeue());
    }
    reply->deleteLater();
    m_hasActiveDownload = false;
    QTimer::singleShot(100, this, &VenuePictures::startNextDownload);
}

bool VenuePictures::startNextDownload()
{
    if (m_hasActiveDownload) {
        QLOG_WARN() << "VenuePictures::startNextDownload(), tried to start new download while one is running";
        return false;
    }
    if (m_downloadQueue.isEmpty()) {
        QLOG_WARN() << "VenuePictures::startNextDownload(), can not start new download, queue is empty";
        return false;
    }

    const VenuePicture &picture = m_downloadQueue.head();
    if (isCached(picture)) {
        QLOG_INFO() << "VenuePictures::startNextDownload(), already in cache! set-ready, dequeue and bail";

        // if picture has been download after being added to the queue and prior to this call
        //  unlikely but to prevent any race conditions
        if (!hasPicture(picture)) {
            setPictureReady(picture);
        }
        // remove and exit
        m_downloadQueue.dequeue();
        QTimer::singleShot(100, this, &VenuePictures::startNextDownload);
    }

    m_nam.get(QNetworkRequest(picture.uri()));
    m_hasActiveDownload = true;

    QLOG_INFO() << "VenuePictures::startNextDownload, download started:" << picture.uri();

    return true;
}

void VenuePictures::onAutoUpdateTimeout()
{
    QLOG_TRACE() << "VenuePictures::onAutoUpdateTimeout()";
    update();
}

bool VenuePictures::hasPicture(const VenuePicture &picture) const
{
    return m_pictures.contains(picture.hash());
}

void VenuePictures::clearPictureList()
{
    m_pictures.clear();
    m_latestPictureCreated = QDateTime();
//    emit pictureListCleared();
}

void VenuePictures::setPictureReady(VenuePicture picture)
{
    picture.setLocalPath(cachePath(picture));
    m_pictures[picture.hash()] = picture;

    if (!m_latestPictureCreated.isValid() || m_latestPictureCreated < picture.createdAt()) {
        m_latestPictureCreated = picture.createdAt();
    }

    QLOG_INFO() << "VenuePictures::setPictureReady() new picture ready:" << picture.localPath();

    emit pictureReady(picture);
}

void VenuePictures::addToDownloadQueue(const VenuePicture &picture)
{
    QLOG_INFO() << "VenuePictures::addToDownloadQueue(" << picture.uri() << ")";

    m_downloadQueue.enqueue(picture);

    if (!m_hasActiveDownload) {
        startNextDownload();
    }
}

bool VenuePictures::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
           || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

bool VenuePictures::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(m_cacheFolder.filePath(filename));
    if (!file.open(QIODevice::WriteOnly)) {
        QLOG_ERROR() << "VenuePictures::saveToDisk(" << filename << "), could not open file for writing\n"
                     << file.errorString();

        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

bool VenuePictures::isCached(const VenuePicture &picture)
{
    return QFile(cachePath(picture)).exists();
}

QString VenuePictures::cachePath(const VenuePicture &picture)
{
    QString filename = QFileInfo(QUrl(picture.uri()).path()).fileName();

    return m_cacheFolder.absoluteFilePath(filename);
}
