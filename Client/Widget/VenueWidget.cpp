#include <QtWidgets/QVBoxLayout>
#include "VenueWidget.h"

VenueWidget::VenueWidget(QWidget *parent) :
    QWidget(parent)
{
    capturePublishWidget = new CapturePublishWidget(this);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(capturePublishWidget);
    setLayout(layout);
}
