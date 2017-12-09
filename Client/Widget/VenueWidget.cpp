#include <QtWidgets/QVBoxLayout>
#include "VenueWidget.h"

VenueWidget::VenueWidget(QWidget *parent) :
    QWidget(parent)
{
    venuePicturesWidget = new VenuePicturesWidget(this);
    capturePublishWidget = new CapturePublishWidget(this);

    auto *layout = new QHBoxLayout(this);
    layout->addWidget(venuePicturesWidget);
    layout->addWidget(capturePublishWidget);
    setLayout(layout);
}
