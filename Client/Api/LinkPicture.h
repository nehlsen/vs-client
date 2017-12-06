#ifndef VSC_LINKPICTURE_H
#define VSC_LINKPICTURE_H

#include "Endpoint.h"

class LinkPicture : public Endpoint
{
public:
    QNetworkRequest createRequest() override;
    QJsonDocument payload() override;

    using Endpoint::isMatch;
    bool isMatch(const QUrl &requestUrl) override;
};

#endif // VSC_LINKPICTURE_H
