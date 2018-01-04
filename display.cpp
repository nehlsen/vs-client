#include <BinCore.h>
#include <QtCore/QTimer>
#include <Client/Widget/SlideShowWidget.h>

#ifndef VSC_FULLSCREEN
#define VSC_FULLSCREEN false
#endif

int main(int argc, char **argv)
{
    BinCore bc(argc, argv);
    bc.init();

    auto *client = bc.createClient();
    auto *slideShow = bc.createSlideShow(client);
    auto *slideShowWidget = new SlideShowWidget(slideShow);

    if (VSC_FULLSCREEN) {
        QTimer::singleShot(0, slideShowWidget, &SlideShowWidget::showFullScreen);
    } else {
        slideShowWidget->show();
    }

    return bc.run();
}
