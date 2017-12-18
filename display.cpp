#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <Client/Client.h>
#include <Client/Settings.h>
#include <Client/Model/VenuePictures.h>
#include <Client/Model/SlideShow.h>
#include <Client/Widget/SlideShowWidget.h>
#include "QsLog/QsLog.h"

using namespace QsLogging;

Client* createClient()
{
    Settings::setDefaults();

    QSettings settings;

    auto *client = new Client();
    client->setServer(settings.value("Server/Url").toString());
    client->venuePictures()->setAutoFetchPicturesEnabled(settings.value("Config/AutoFetchEnabled").toBool());
    client->venuePictures()->setAutoUpdateInterval(settings.value("Config/AutoUpdateInterval").toInt());
    client->venuePictures()->setCacheFolder(settings.value("Local/CacheFolder").toString());

    QString venue = settings.value("Server/Venue").toString();
    QObject::connect(
            client,
            &Client::acquireTokenSucceed,
            [=]() { client->setVenue(venue); }
    );

    client->acquireToken(
            settings.value("Server/Username").toString(),
            settings.value("Server/Password").toString()
    );

    return client;
}

SlideShow *createSlideShow(Client *client)
{
    QSettings settings;

    auto *slideShow = new SlideShow(client);
    slideShow->setInterval(settings.value("SlideShow/Interval").toInt());

    return slideShow;
}

int main(int argc, char **argv)
{
    QApplication::setOrganizationName("nehlsen");
    QApplication::setOrganizationDomain("nehlsen.org");
    QApplication::setApplicationName("VenueShotClient");

    QApplication app(argc, argv);

    // 1. init the logging mechanism
    Logger& logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(app.applicationDirPath()).filePath("vsc.log"));

    // 2. add two destinations
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
            sLogPath, EnableLogRotation, MaxSizeBytes(1024*1024), MaxOldLogCount(2)));
    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
//    DestinationPtr functorDestination(DestinationFactory::MakeFunctorDestination(&logFunction));
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);
//    logger.addDestination(functorDestination);

    auto *client = createClient();
    auto *slideShow = createSlideShow(client);
    auto *slideShowWidget = new SlideShowWidget(slideShow);
//    slideShowWidget->showFullScreen();
    slideShowWidget->show();

    int exitCode = app.exec();
    Logger::destroyInstance();
    return exitCode;
}
