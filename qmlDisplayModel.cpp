#include <BinCore.h>
#include <QtCore/QTimer>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>
#include <Client/Model/VenuePicturesModel.h>

#ifndef VSC_FULLSCREEN
#define VSC_FULLSCREEN false
#endif

int main(int argc, char **argv)
{
    BinCore bc(argc, argv);
    bc.init();

    auto *client = bc.createClient();
    auto *venuePicturesModel = new VenuePicturesModel(client->venuePictures());

    auto mainView = new QQuickView;
    mainView->rootContext()->setContextProperty("venuePicturesModel", venuePicturesModel);
    mainView->setSource(QUrl::fromLocalFile("qml/displayModel.qml"));
    if (VSC_FULLSCREEN) {
        QTimer::singleShot(0, mainView, &QQuickView::showFullScreen);
    } else {
        mainView->show();
    }

    return bc.run();
}
