#include "VenuePictures.h"
#include <Client/Client.h>
#include <QsLog/QsLog.h>
#include <QtCore/QTimer>

VenuePictures::VenuePictures(Client *client):
    m_client(client),
    m_autoFetchPicturesEnabled(false),
    m_hasActiveDownload(false)
{
    setCacheFolder(QDir::tempPath() + "/VenueShot/Cache");

    connect(m_client, &Client::venueChanged,
            this, &VenuePictures::onVenueChanged);

    connect(&m_nam, &QNetworkAccessManager::finished,
            this, &VenuePictures::onDownloadFinished);
}

void VenuePictures::setAutoFetchPicturesEnabled(bool enabled)
{
    m_autoFetchPicturesEnabled = enabled;
}

bool VenuePictures::isAutoFetchPicturesEnabled() const
{
    return m_autoFetchPicturesEnabled;
}

void VenuePictures::setCacheFolder(const QString &folder)
{
    m_cacheFolder.setCurrent(folder);

    if (!m_cacheFolder.exists()) {
        m_cacheFolder.mkpath(folder);
    }

    Q_ASSERT(m_cacheFolder.exists());

    QLOG_INFO() << "VenuePictures::setCacheFolder(), saving cache files to: " << m_cacheFolder.absolutePath();
}

void VenuePictures::update()
{
    m_client->getVenuePictures();
}

void VenuePictures::readUpdate(const QList<VenuePicture> &pictureList)
{
    // for now just replace
    //  later on try to calculate diffs and issue signals appropriately
//    m_pictureList = pictureList;
//    emit picturesAdded(m_pictureList);

    QLOG_INFO() << "VenuePictures::readUpdate, count:" << pictureList.size();

    for (VenuePicture vp : pictureList) {
        if (isCached(vp)) {
            setPictureReady(vp);
        } else {
            addToDownloadQueue(vp);
        }
    }
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
        QLOG_WARN() << "VenuePictures::startNextDownload, tried to start new download while one is running";
        return false;
    }
    if (m_downloadQueue.isEmpty()) {
        QLOG_WARN() << "VenuePictures::startNextDownload, can not start new download, queue is empty";
        return false;
    }

    const VenuePicture &vp = m_downloadQueue.head();
    QNetworkRequest request(vp.uri());
    QNetworkReply *reply = m_nam.get(request);
    m_hasActiveDownload = true;

    QLOG_INFO() << "VenuePictures::startNextDownload, download started:" << vp.uri();

    return true;
}

void VenuePictures::clearPictureList()
{
    m_pictureList.clear();
//    emit pictureListCleared();
}

void VenuePictures::setPictureReady(VenuePicture picture)
{
    picture.setLocalPath(cachePath(picture));
    m_pictureList << picture;
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
