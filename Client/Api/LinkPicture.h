#ifndef VSC_LINKPICTURE_H
#define VSC_LINKPICTURE_H

#include "Endpoint.h"

class LinkPicture : public Endpoint
{
public:
    QNetworkRequest createRequest() override;
    QJsonDocument payload() override;

protected:
    bool handleJsonDocument(const QJsonDocument &document) override;
};

#endif // VSC_LINKPICTURE_H
