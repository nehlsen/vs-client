#ifndef VSC_GETVENUEPICTURES_H
#define VSC_GETVENUEPICTURES_H

#include <Client/Model/VenuePicture.h>
#include "Endpoint.h"

class GetVenuePictures : public Endpoint
{
public:
    QNetworkRequest createRequest() override;

    using Endpoint::isMatch;
    bool isMatch(const QUrl &requestUrl) override;

    QList<VenuePicture> venuePictures();

protected:
    bool handleJsonDocument(const QJsonDocument &document) override;

    QList<VenuePicture> m_venuePictures;
};

#endif // VSC_GETVENUEPICTURES_H
