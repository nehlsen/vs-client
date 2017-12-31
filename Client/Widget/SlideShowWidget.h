#ifndef VSC_SLIDESHOWWIDGET_H
#define VSC_SLIDESHOWWIDGET_H

#include <QtWidgets/QWidget>
#include <Client/Model/VenuePicture.h>

class SlideShow;
class Client;
class QLabel;

class SlideShowWidget : public QWidget
{
Q_OBJECT

public:
    explicit SlideShowWidget(SlideShow *slideShow, QWidget *parent = nullptr);
    explicit SlideShowWidget(Client *client, QWidget *parent = nullptr);

public slots:
    void reset();
    void onShowPicture(const VenuePicture *picture);

protected:
    void initLayout();
    QPixmap m_currentPicture;
    QLabel *m_pictureDisplay;

    void setupSlideShow(SlideShow *slideShow);
};

#endif // VSC_SLIDESHOWWIDGET_H
