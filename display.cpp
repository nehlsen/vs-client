#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <Client/Client.h>
#include <Client/Widget/VenuePicturesWidget.h>
#include <Client/Settings.h>
#include "QsLog/QsLog.h"

using namespace QsLogging;

Client* createClient()
{
    Settings::setDefaults();

    QSettings settings;

    auto *client = new Client();
    client->setServer(settings.value("Server/Url").toString());
    client->setAutoFetchPicturesEnabled(settings.value("Config/AutoFetchEnabled").toBool());

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
    auto *display = new VenuePicturesWidget();
    display->setVenuePictures(client->venuePictures());
//    display->showFullScreen();
    display->show();

    int exitCode = app.exec();
    Logger::destroyInstance();
    return exitCode;
}
