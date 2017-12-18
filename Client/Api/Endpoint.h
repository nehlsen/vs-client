#ifndef VSC_ENDPOINT_H
#define VSC_ENDPOINT_H

#include <QtCore/QJsonDocument>
#include "BasicEndpoint.h"

class Endpoint : public BasicEndpoint
{
public:
    virtual QJsonDocument payload();

    using BasicEndpoint::parseResponse;
    bool parseResponse(int httpStatusCode, const QByteArray &responseBody) override;

protected:
    virtual bool handleJsonDocument(const QJsonDocument &document) = 0;
    void addJsonContentHeader(QNetworkRequest &request);
};

#endif // VSC_ENDPOINT_H
