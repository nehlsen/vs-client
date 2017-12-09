#ifndef VSC_VENUEWIDGET_H
#define VSC_VENUEWIDGET_H

#include <QtWidgets/QWidget>
#include "CapturePublishWidget.h"
#include "VenuePicturesWidget.h"

class VenueWidget : public QWidget
{
Q_OBJECT

public:
    explicit VenueWidget(QWidget *parent = nullptr);

    // display venue title
    // display venue pictures (extra widget)
    // grab and upload picture (extra widget)

    VenuePicturesWidget *venuePicturesWidget;
    CapturePublishWidget *capturePublishWidget;
};

#endif // VSC_VENUEWIDGET_H
