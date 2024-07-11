#ifndef VENUESHOTCLIENT_KRATOSSTARTFLOW_H
#define VENUESHOTCLIENT_KRATOSSTARTFLOW_H

#include "Endpoint.h"

class KratosStartFlow : public Endpoint
{
public:
    QNetworkRequest createRequest() override;

    const QString &actionUrl() const;
protected:
    bool handleJsonDocument(const QJsonDocument &document) override;

private:
    QString m_actionUrl;

    static QString extractActionUrl(const QJsonObject &jsonObject);
};

#endif //VENUESHOTCLIENT_KRATOSSTARTFLOW_H
