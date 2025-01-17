#ifndef VSC_VENUESHOT_H
#define VSC_VENUESHOT_H

#include <QtCore/QObject>
#include <Client/Client.h>
#include <Client/Model/SlideShow.h>
#include <Client/Model/Venue.h>
#include <Client/Model/VenuePicture.h>

class VenueShot : public QObject
{
Q_OBJECT
// properties mirrored from client
Q_PROPERTY(QString serverAuthentication READ serverAuthentication WRITE setServerAuthentication NOTIFY serverAuthenticationChanged)
Q_PROPERTY(QString serverService READ serverService WRITE setServerService NOTIFY serverServiceChanged)
Q_PROPERTY(QString serverStorage READ serverStorage WRITE setServerStorage NOTIFY serverStorageChanged)
Q_PROPERTY(Client::Status status READ status /*WRITE setStatus*/ NOTIFY statusChanged)
Q_PROPERTY(QString venueToken READ venueToken WRITE setVenueToken NOTIFY venueTokenChanged)
Q_PROPERTY(Venue* venue READ venue WRITE setVenue NOTIFY venueChanged)
// properties mirrored from VenuePictures
Q_PROPERTY(bool autoFetchPicturesEnabled READ isAutoFetchPicturesEnabled WRITE setAutoFetchPicturesEnabled NOTIFY autoFetchPicturesEnabledChanged)
Q_PROPERTY(bool autoUpdateEnabled READ isAutoUpdateEnabled WRITE setAutoUpdateEnabled NOTIFY autoUpdateEnabledChanged)
Q_PROPERTY(int autoUpdateInterval READ autoUpdateInterval WRITE setAutoUpdateInterval NOTIFY autoUpdateIntervalChanged)
Q_PROPERTY(QString cacheFolder READ cacheFolder WRITE setCacheFolder NOTIFY cacheFolderChanged)
// properties mirrored from SlideShow
Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
Q_PROPERTY(VenuePicture* currentPicture READ currentPicture /*WRITE setCurrentPicture*/ NOTIFY currentPictureChanged)

public:
    static void declareQml();
    explicit VenueShot(QObject *parent = nullptr);

    QString serverAuthentication() const;
    QString serverService() const;
    QString serverStorage() const;
    Client::Status status() const;
    QString venueToken() const;
    Venue* venue() const;

    bool isAutoFetchPicturesEnabled() const;
    bool isAutoUpdateEnabled() const;
    int autoUpdateInterval() const;
    QString cacheFolder() const;

    int interval() const;
    VenuePicture* currentPicture() const;

public slots:
    void setServerAuthentication(const QString &uri);
    void setServerService(const QString &uri);
    void setServerStorage(const QString &uri);
    void acquireToken(const QString &username, const QString &password);
    void postPicture(const QImage &image);
    void postPicture(const QString &imageUri);
    //    void setStatus();
    void setVenueToken(const QString &token);
    void setVenue(Venue *venue);

    void setAutoFetchPicturesEnabled(bool enabled);
    void setAutoUpdateEnabled(bool enabled);
    void setAutoUpdateInterval(int seconds);
    void setCacheFolder(const QString &folder);

    void setInterval(int interval);
//    void setCurrentPicture();

signals:
    void serverAuthenticationChanged();
    void serverServiceChanged();
    void serverStorageChanged();
    void statusChanged(Client::Status newStatus);
    void venueTokenChanged();
    void venueChanged();

    void autoFetchPicturesEnabledChanged();
    void autoUpdateEnabledChanged();
    void autoUpdateIntervalChanged();
    void cacheFolderChanged();

    void intervalChanged();
    void currentPictureChanged(const VenuePicture *newPicture);

private:
    Client *m_client;
    SlideShow *m_slideShow;
};

#endif // VSC_VENUESHOT_H
