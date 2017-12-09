#ifndef VSC_VENUEPICTURES_H
#define VSC_VENUEPICTURES_H

#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QQueue>
#include <QtNetwork/QNetworkAccessManager>
#include "Venue.h"
#include "VenuePicture.h"

class Client;

class VenuePictures : public QObject
{
Q_OBJECT

public:
    explicit VenuePictures(Client *client);

    void setAutoFetchPicturesEnabled(bool enabled = true);
    bool isAutoFetchPicturesEnabled() const;

    void setCacheFolder(const QString &folder);

signals:
//    void pictureListCleared();
//    void picturesAdded(const QList<VenuePicture> &pictures);
    void pictureReady(const VenuePicture &pictures);

public slots:
    void update();
    void readUpdate(const QList<VenuePicture> &pictureList);

protected slots:
    void onVenueChanged(const Venue &venue);
    void onDownloadFinished(QNetworkReply *reply);
    bool startNextDownload();

protected:
    Client *m_client;
    bool m_autoFetchPicturesEnabled;

    QDir m_cacheFolder;

    QList<VenuePicture> m_pictureList;
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
};

#endif // VSC_VENUEPICTURES_H
