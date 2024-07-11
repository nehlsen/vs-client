#include "KratosStartFlow.h"
#include <QsLog/QsLog.h>
#include <QtCore/QJsonObject>

#define ACQUIRE_TOKEN_URL "/self-service/login/api"

QNetworkRequest KratosStartFlow::createRequest()
{
    QNetworkRequest request(QUrl(server() + ACQUIRE_TOKEN_URL));
    addJsonContentHeader(request);

    return request;
}

const QString &KratosStartFlow::actionUrl() const
{
    return m_actionUrl;
}

bool KratosStartFlow::handleJsonDocument(const QJsonDocument &document)
{
    m_actionUrl = extractActionUrl(document.object());

    return !m_actionUrl.isEmpty();
}

QString KratosStartFlow::extractActionUrl(const QJsonObject &jsonObject)
{
    QLOG_TRACE() << "KratosStartFlow::extractActionUrl()";

    if (!jsonObject.contains(QLatin1String("ui"))) {
        QLOG_ERROR() << "KratosSessionToken::fromJsonObject(): missing ui";
        return QLatin1String();
    }

    const QJsonObject &ui = jsonObject.value(QLatin1String("ui")).toObject();
    if (!ui.contains(QLatin1String("action"))) {
        QLOG_ERROR() << "KratosSessionToken::fromJsonObject(): missing ui.action";
        return QLatin1String();
    }

    return ui.value(QLatin1String("action")).toString();
}
