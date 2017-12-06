#include "BasicEndpoint.h"
#include <QtNetwork/QNetworkReply>
#include <QsLog/QsLog.h>

void BasicEndpoint::setRequestParameters(const QString &server, const QStringList &parameters)
{
    m_server = server;
    m_requestParameters = parameters;
}

QString BasicEndpoint::server() const
{
    return m_server;
}

QStringList BasicEndpoint::requestParameters() const
{
    return m_requestParameters;
}

bool BasicEndpoint::isMatch(QNetworkReply *reply)
{
    const QUrl &requestUrl = reply->request().url();

    return isMatch(requestUrl);
}

bool BasicEndpoint::parseResponse(QNetworkReply *reply)
{
    QLOG_TRACE() << "BasicEndpoint::parseResponse(reply)";

    const int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QUrl &requestUrl = reply->request().url();

    QLOG_INFO() << requestUrl.toString()
                << httpStatusCode
                << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

            foreach(QByteArray key, reply->rawHeaderList()) {
            QLOG_DEBUG() << "\t" << key << reply->rawHeader(key);
        }

    const QByteArray responseBody = reply->readAll();
    QLOG_TRACE() << responseBody;

    return parseResponse(httpStatusCode, responseBody);
}
