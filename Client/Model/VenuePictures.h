#ifndef VSC_VENUEPICTURES_H
#define VSC_VENUEPICTURES_H

#include <QtCore/QObject>
#include "Venue.h"
#include "VenuePicture.h"

class Client;

class VenuePictures : public QObject
{
Q_OBJECT

public:
    explicit VenuePictures(Client *client);

    void setAutoFetchPicturesEnabled(bool enabled = true);
    bool isAutoFetchPicturesEnabled() const;

signals:
    void pictureListCleared();
    void picturesAdded(const QList<VenuePicture> &pictures);

public slots:
    void update();
    void readUpdate(const QList<VenuePicture> &pictureList);

protected slots:
    void onVenueChanged(const Venue &venue);

protected:
    Client *m_client;
    bool m_autoFetchPicturesEnabled;

    QList<VenuePicture> m_pictureList;
    void clearPictureList();
};

#endif // VSC_VENUEPICTURES_H
