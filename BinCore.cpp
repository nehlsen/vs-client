#include "BinCore.h"
#include <QtCore/QSettings>
#include <Client/Settings.h>
#include <Client/Widget/SlideShowWidget.h>
#include "QsLog/QsLog.h"

using namespace QsLogging;

BinCore::BinCore(int argc, char **argv):
    m_app(argc, argv)
{
}

void BinCore::init(InitOptions options)
{
    QApplication::setOrganizationName("nehlsen");
    QApplication::setOrganizationDomain("nehlsen.org");
    QApplication::setApplicationName("VenueShotClient");

    Logger& logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(m_app.applicationDirPath()).filePath("vsc.log"));

    if (options | LogCli) {
        DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
        logger.addDestination(debugDestination);
    }

    if (options | LogFile) {
        DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
                sLogPath, EnableLogRotation, MaxSizeBytes(1024*1024), MaxOldLogCount(2)));
        logger.addDestination(fileDestination);
    }
}

int BinCore::run()
{
    int exitCode = m_app.exec();
    Logger::destroyInstance();
    return exitCode;
}

Client* BinCore::createClient()
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

SlideShow *BinCore::createSlideShow(Client *client)
{
    QSettings settings;

    auto *slideShow = new SlideShow(client);
    slideShow->setInterval(settings.value("SlideShow/Interval").toInt());

    return slideShow;
}
