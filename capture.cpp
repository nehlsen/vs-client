#include <BinCore.h>
#include <QtCore/QTimer>
#include <Client/Widget/CapturePublishWidget.h>

#ifndef VSC_FULLSCREEN
#define VSC_FULLSCREEN false
#endif

int main(int argc, char **argv)
{
    BinCore bc(argc, argv);
    bc.init();

    auto *client = bc.createClient();
    auto capturePublishWidget = new CapturePublishWidget();
    QObject::connect(
            capturePublishWidget,
            &CapturePublishWidget::publishImage,
            client,
            &Client::postPicture
    );

    if (VSC_FULLSCREEN) {
        QTimer::singleShot(0, capturePublishWidget, &CapturePublishWidget::showFullScreen);
    } else {
        capturePublishWidget->show();
    }

    return bc.run();
}
