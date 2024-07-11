#ifndef VENUESHOTCLIENT_KRATOSAUTHENTICATE_H
#define VENUESHOTCLIENT_KRATOSAUTHENTICATE_H

#include "Endpoint.h"
#include <Client/KratosSessionToken.h>

class KratosAuthenticate : public Endpoint
{
public:
    QNetworkRequest createRequest() override;
    QJsonDocument payload() override;

    KratosSessionToken token() const;
protected:
    bool handleJsonDocument(const QJsonDocument &document) override;

private:
    KratosSessionToken m_token;
};

#endif //VENUESHOTCLIENT_KRATOSAUTHENTICATE_H
