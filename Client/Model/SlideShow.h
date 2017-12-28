#ifndef VSC_SLIDESHOW_H
#define VSC_SLIDESHOW_H

#include <QtCore/QObject>
#include <Client/Model/VenuePictures.h>

class QTimer;

/**
 * collects downloaded, ready pictures, governs which picture to display
 * sits on top of VenuePictures and listens for the picture-ready signal to collect pictures
 * TODO: implement some form of weighted random list or any other algorithm to control the flow of pictures
 */
class SlideShow : public QObject
{
Q_OBJECT
Q_PROPERTY(VenuePicture currentPicture READ currentPicture NOTIFY showPicture)

public:
    explicit SlideShow(QObject *parent = nullptr);
    explicit SlideShow(Client *client);

    // set slideshow interval in milli seconds, intervals less than 1s are changed to 1s
    void setInterval(int msec);
    int interval() const;

    VenuePicture currentPicture() const;
    int currentPictureIndex() const;
    int countPictures() const;

signals:
    void cleared();
    void showPicture(const VenuePicture &picture);

public slots:
    void onPictureListCleared();
    void onPictureReady(const VenuePicture &picture);

protected slots:
    void advancePicture();

protected:
    int m_picturesCurrentIndex;
    QList<VenuePicture> m_pictures;

    void initTimer();
    QTimer *m_delayAdvancePicture;
};

#endif // VSC_SLIDESHOW_H
