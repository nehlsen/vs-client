#ifndef VSC_ACQUIRETOKEN_H
#define VSC_ACQUIRETOKEN_H

#include "Endpoint.h"
#include <Client/JwtToken.h>

// FIXME probably not wise to save password ?!
class AcquireToken : public Endpoint
{
public:
    QNetworkRequest createRequest() override;
    QJsonDocument payload() override;

    using Endpoint::isMatch;
    bool isMatch(const QUrl &requestUrl) override;

    JwtToken token() const;
protected:
    bool handleJsonDocument(const QJsonDocument &document) override;

private:
    JwtToken m_token;
};

#endif // VSC_ACQUIRETOKEN_H
