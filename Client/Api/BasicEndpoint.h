#ifndef VSC_BASICENDPOINT_H
#define VSC_BASICENDPOINT_H

#include <QtCore/QUrl>
#include <QtNetwork/QNetworkRequest>

#define DEV_MODE_PREFIX "/app_dev.php"

class QNetworkReply;

class BasicEndpoint
{
public:
    void setRequestParameters(const QString &server, const QStringList &parameters);
    QString server() const;
    QStringList requestParameters() const;
    virtual QNetworkRequest createRequest() = 0;

    virtual bool isMatch(QNetworkReply *reply);
    virtual bool isMatch(const QUrl &requestUrl) = 0;
    virtual bool parseResponse(QNetworkReply *reply);
    virtual bool parseResponse(int httpStatusCode, const QByteArray &responseBody) = 0;

private:
    QString m_server;
    QStringList m_requestParameters;
};

#endif // VSC_BASICENDPOINT_H