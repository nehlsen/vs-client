#ifndef VSC_CLIENT_H
#define VSC_CLIENT_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "JwtToken.h"
#include <Client/Api/GetVenue.h>
#include <Client/Api/AcquireToken.h>

class QTimer;

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

    explicit Client(QObject *parent = nullptr);

    void setServer(const QString &server);
    QString server() const;

    void setUsername(const QString& username);
    QString username() const;

    void setPassword(const QString& password);
    QString password() const;

    void setTokenAutoRefreshEnabled(bool enabled = true);
    bool isTokenAutoRefreshEnabled() const;

    Status status() const;

    JwtToken token() const;

    void setVenue(const QString &venueToken);
    Venue venue() const;

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
    void acquireToken();
    void acquireToken(const QString &username, const QString &password);

signals:
    void tokenChanged(const JwtToken &token);
    void acquireTokenSucceed();
    void acquireTokenFailed();

    void venueChanged(const Venue &venue);

    void statusChanged(Status status);

protected slots:
    void requestFinished();
    void requestError(QNetworkReply::NetworkError code);
    void requestSslErrors(const QList<QSslError> &errors);

protected:
    QString m_server;
    QString m_username;
    QString m_password; // FIXME probably not wise to save password ?!

    bool m_autoRefreshEnabled;
    JwtToken m_token;
    void setToken(const JwtToken &token);

    Venue m_venue;
    void setVenue(const Venue &venue);

    void updateStatus();

    AcquireToken m_endpointAcquireToken;
    GetVenue m_endpointGetVenue;

    QNetworkAccessManager m_qnam;

    QNetworkReply *post(const QNetworkRequest &request, const QJsonObject &payload);
    QNetworkReply *post(const QNetworkRequest &request, const QJsonDocument &payload);
    QNetworkReply *post(const QNetworkRequest &request, const QByteArray &payload);
    QNetworkReply *get(QNetworkRequest request);
    QNetworkRequest addAuthHeader(QNetworkRequest request);
    QNetworkReply *prepareReply(QNetworkReply *reply);
    void dumpRequestInfo(const QString &verb, const QNetworkRequest &request) const; };

#endif // VSC_CLIENT_H
