#ifndef VSC_ENDPOINT_H
#define VSC_ENDPOINT_H

#include <QtCore/QJsonDocument>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkRequest>

#define DEV_MODE_PREFIX "/app_dev.php"

class Endpoint
{
public:
    void setRequestParameters(const QString &server, const QStringList &parameters);
    QString server() const;
    QStringList requestParameters() const;
    virtual QNetworkRequest createRequest() = 0;
    virtual QJsonDocument payload();

    virtual bool isMatch(const QUrl &requestUrl) = 0;
    bool parseResponse(int httpStatusCode, const QByteArray &responseBody);

protected:
    virtual bool handleJsonDocument(const QJsonDocument &document);
    void addContentHeader(QNetworkRequest &request);

private:
    QString m_server;
    QStringList m_requestParameters;
};

#endif // VSC_ENDPOINT_H
