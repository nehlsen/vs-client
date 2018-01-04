#include <BinCore.h>
#include <QtCore/QTimer>
#include <Client/VenueShot.h>
#include <QtQuick/QQuickView>

#ifndef VSC_FULLSCREEN
#define VSC_FULLSCREEN false
#endif

int main(int argc, char **argv)
{
    BinCore bc(argc, argv);
    bc.init();

    VenueShot::declareQml();

    auto mainView = new QQuickView;
    mainView->setSource(QUrl::fromLocalFile("qml/capture.qml"));
    if (VSC_FULLSCREEN) {
        QTimer::singleShot(0, mainView, &QQuickView::showFullScreen);
    } else {
        mainView->show();
    }

    return bc.run();
}
