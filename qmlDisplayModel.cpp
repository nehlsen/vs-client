#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>
#include <Client/Client.h>
#include <Client/Settings.h>
#include <Client/Model/VenuePictures.h>
#include <Client/Widget/SlideShowWidget.h>
#include <Client/Model/VenuePicturesModel.h>
#include "QsLog/QsLog.h"

#ifndef VSC_FULLSCREEN
#define VSC_FULLSCREEN false
#endif

using namespace QsLogging;

Client* createClient()
{
    Settings::setDefaults();

    QSettings settings;

    auto *client = new Client();
    client->setServer(settings.value("url").toString());
    client->venuePictures()->setAutoFetchPicturesEnabled(settings.value("autoFetchEnabled").toBool());
    client->venuePictures()->setAutoUpdateInterval(settings.value("autoUpdateInterval").toInt());
    client->venuePictures()->setCacheFolder(settings.value("cacheFolder").toString());

    QString venue = settings.value("venue").toString();
    QObject::connect(
            client,
            &Client::acquireTokenSucceed,
            [=]() { client->setVenue(venue); }
    );

    client->acquireToken(
            settings.value("username").toString(),
            settings.value("password").toString()
    );

    return client;
}

int main(int argc, char **argv)
{
    QApplication::setOrganizationName("nehlsen");
    QApplication::setOrganizationDomain("nehlsen.org");
    QApplication::setApplicationName("VenueShotClient");

    QApplication app(argc, argv);

    Logger& logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(app.applicationDirPath()).filePath("vsc.log"));

    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
            sLogPath, EnableLogRotation, MaxSizeBytes(1024*1024), MaxOldLogCount(2)));
    logger.addDestination(fileDestination);

#ifdef VSC_CLI_LOG
    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(debugDestination);
#endif

    // ------------------------------------------------------------------------------------------------

    auto *client = createClient();
    auto *venuePicturesModel = new VenuePicturesModel(client->venuePictures());

    QQuickView mainView;
    mainView.rootContext()->setContextProperty("venuePicturesModel", venuePicturesModel);
    mainView.setSource(QUrl::fromLocalFile("qml/displayModel.qml"));
    if (VSC_FULLSCREEN) {
        mainView.showFullScreen();
    } else {
        mainView.show();
    }

    int exitCode = app.exec();
    Logger::destroyInstance();
    return exitCode;
}
