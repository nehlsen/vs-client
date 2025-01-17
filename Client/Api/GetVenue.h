#ifndef VSC_GETVENUE_H
#define VSC_GETVENUE_H

#include <Client/Model/Venue.h>
#include "Endpoint.h"

class GetVenue : public Endpoint
{
public:
    GetVenue();

    QNetworkRequest createRequest() override;
    Venue* venue() const;

protected:
    bool handleJsonDocument(const QJsonDocument &document) override;

private:
    Venue* m_venue;
};

#endif // VSC_GETVENUE_H
