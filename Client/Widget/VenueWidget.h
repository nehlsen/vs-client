#ifndef VSC_VENUEWIDGET_H
#define VSC_VENUEWIDGET_H

#include <QtWidgets/QWidget>
#include "CapturePublishWidget.h"

class SlideShowWidget;
class Client;

class VenueWidget : public QWidget
{
Q_OBJECT

public:
    explicit VenueWidget(Client *client, QWidget *parent = nullptr);

    // display venue title
    // display venue pictures (extra widget)
    // grab and upload picture (extra widget)

    SlideShowWidget *slideShowWidget;
    CapturePublishWidget *capturePublishWidget;
};

#endif // VSC_VENUEWIDGET_H
