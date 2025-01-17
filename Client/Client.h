#ifndef VSC_CLIENT_H
#define VSC_CLIENT_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <Client/Api/GetVenue.h>
#include <Client/Api/GetVenuePictures.h>
#include <Client/Api/PostPicture.h>
#include <Client/Api/LinkPicture.h>
#include <Client/Api/KratosStartFlow.h>
#include <Client/Api/KratosAuthenticate.h>

class VenuePictures;

class Client : public QObject
{
Q_OBJECT

public:
    enum Status {
        Offline,
        Online,
        Ready,
        Error
    };
    Q_ENUMS(Status)

    explicit Client(QObject *parent = nullptr);

    void setServerAuthentication(const QString &server);
    QString serverAuthentication() const;

    void setServerService(const QString &server);
    QString serverService() const;

    void setServerStorage(const QString &server);
    QString serverStorage() const;

    Status status() const;

    KratosSessionToken token() const;

    void setVenue(const QString &venueToken);
    Venue *venue() const;
    VenuePictures *venuePictures();

public slots:
    /**
     * acquire a token using provided username and password
     *
     * try to acquire a new token using the provided username and password.
     * if auto refresh is enabled, the token will be automatically refreshed.
     *
     * @param username
     * @param password
     * @see tokenChanged
     */
    void acquireToken(const QString &username, const QString &password); // FIXME rename to login

    void postPicture(const QImage &image);
    void linkPicture(const QString &publicPictureLocation);

    /**
     * retrieve Pictures for current venue
     */
    void getVenuePictures();
    void getVenuePictures(const QDateTime &createdAfter);

signals:
    void tokenChanged(const KratosSessionToken &token);
    void acquireTokenSucceed();
    void acquireTokenFailed();

    void venueChanged(const Venue *venue);

    void pictureUploaded(const QString &publicPictureLocation);
    void pictureLinked();

    void statusChanged(Status status);

protected slots:
    void requestFinished();
    void requestError(QNetworkReply::NetworkError code);
    void requestSslErrors(const QList<QSslError> &errors);

protected:
    QString m_serverAuthentication;
    QString m_serverService;
    QString m_serverStorage;

    void continueAuthentication(const QString &actionUrl);
    KratosSessionToken m_token;
    void setToken(const KratosSessionToken &token);

    Venue *m_venue;
    void setVenue(Venue *venue);
    VenuePictures *m_venuePictures;

    void updateStatus();

    KratosStartFlow m_endpointKratosStartFlow;
    KratosAuthenticate m_endpointKratosAuthenticate;
    GetVenue m_endpointGetVenue;
    PostPicture m_endpointPostPicture;
    LinkPicture m_endpointLinkPicture;
    GetVenuePictures m_endpointGetVenuePictures;

    QNetworkAccessManager m_qnam;

    QNetworkReply *post(const QNetworkRequest &request, const QJsonObject &payload);
    QNetworkReply *post(const QNetworkRequest &request, const QJsonDocument &payload);
    QNetworkReply *post(const QNetworkRequest &request, const QByteArray &payload);
    QNetworkReply *post(const QNetworkRequest &request, QHttpMultiPart *payload);
    QNetworkReply *get(QNetworkRequest request);
    QNetworkRequest addAuthHeader(QNetworkRequest request);
    QNetworkReply *prepareReply(QNetworkReply *reply);
    void dumpRequestInfo(const QString &verb, const QNetworkRequest &request) const;
};

#endif // VSC_CLIENT_H
