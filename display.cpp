#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <Client/Client.h>
#include <Client/Model/VenuePictures.h>
#include <Client/Model/SlideShow.h>
#include <Client/Widget/SlideShowWidget.h>
#include <QtCore/QTimer>
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include <Client/Settings.h>

#ifndef VSC_FULLSCREEN
#define VSC_FULLSCREEN false
#endif

using namespace QsLogging;

Client* createClient()
{
    Settings::setDefaults();

    QSettings settings;

    auto *client = new Client();
    client->setServerService(settings.value("serverService").toString());
    client->setServerStorage(settings.value("serverStorage").toString());
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

SlideShow *createSlideShow(Client *client)
{
    QSettings settings;

    auto *slideShow = new SlideShow(client);
    slideShow->setInterval(settings.value("slideShowInterval").toInt());

    return slideShow;
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
            sLogPath, LogRotationOption::EnableLogRotation, MaxSizeBytes(1024*1024), MaxOldLogCount(2)));
    logger.addDestination(std::move(fileDestination));

#ifdef VSC_CLI_LOG
    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(std::move(debugDestination));
#endif

    // ------------------------------------------------------------------------------------------------

    auto *client = createClient();
    auto *slideShow = createSlideShow(client);
    auto *slideShowWidget = new SlideShowWidget(slideShow);

    if (VSC_FULLSCREEN) {
        QTimer::singleShot(0, slideShowWidget, &SlideShowWidget::showFullScreen);
    } else {
        slideShowWidget->show();
    }

    return app.exec();
}
