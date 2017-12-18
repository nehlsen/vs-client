#ifndef VSC_VENUEPICTURES_H
#define VSC_VENUEPICTURES_H

#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QQueue>
#include <QtNetwork/QNetworkAccessManager>
#include "Venue.h"
#include "VenuePicture.h"

class Client;
class QTimer;

/**
 * fetches the list of pictures of a venue, download and caches the binary image data
 * (knows all pictures of a venue, downloaded or not)
 */
class VenuePictures : public QObject
{
Q_OBJECT

public:
    explicit VenuePictures(Client *client);

    // once venue changed is receive, the internal picture list is cleared
    //  if auto fetch is enabled it will be automatically be re populated
    void setAutoFetchPicturesEnabled(bool enabled = true);
    bool isAutoFetchPicturesEnabled() const;

    void setAutoUpdateInterval(int seconds);
    int autoUpdateInterval() const;
    bool isAutoUpdateEnabled() const;

    void setCacheFolder(const QString &folder);

signals:
    void pictureListCleared();
//    void picturesAdded(const QList<VenuePicture> &pictures);
    void pictureReady(const VenuePicture &pictures);

public slots:
    void update();
    void readUpdate(const QList<VenuePicture> &pictureList);

protected slots:
    void onVenueChanged(const Venue &venue);
    void onDownloadFinished(QNetworkReply *reply);
    bool startNextDownload();
    void onAutoUpdateTimeout();

protected:
    Client *m_client;
    bool m_autoFetchPicturesEnabled;

    QDir m_cacheFolder;

    QMap<QString,VenuePicture> m_pictures;
    QDateTime m_latestPictureCreated;
    bool hasPicture(const VenuePicture &picture) const;
    void clearPictureList();
    void setPictureReady(VenuePicture picture);

    void addToDownloadQueue(const VenuePicture &picture);
    QQueue<VenuePicture> m_downloadQueue;

    QNetworkAccessManager m_nam;
    bool m_hasActiveDownload;
    bool isHttpRedirect(QNetworkReply *reply);
    bool saveToDisk(const QString &filename, QIODevice *data);

    bool isCached(const VenuePicture &picture);
    QString cachePath(const VenuePicture &picture);

    bool m_updateIsRunning;
    QTimer *m_autoUpdate;
};

#endif // VSC_VENUEPICTURES_H
