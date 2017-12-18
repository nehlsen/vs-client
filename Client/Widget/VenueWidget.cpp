#include "VenueWidget.h"
#include <QtWidgets/QVBoxLayout>
#include <Client/Widget/SlideShowWidget.h>

VenueWidget::VenueWidget(Client *client, QWidget *parent) :
    QWidget(parent)
{
    slideShowWidget = new SlideShowWidget(client, this);
    capturePublishWidget = new CapturePublishWidget(this);

    auto *layout = new QHBoxLayout(this);
    layout->addWidget(slideShowWidget);
    layout->addWidget(capturePublishWidget);
    setLayout(layout);
}
