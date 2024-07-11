#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <Client/Client.h>
#include <Client/Widget/MainWindow.h>
#include "QsLog/QsLog.h"
#include <Client/Settings.h>

using namespace QsLogging;

Client* createClient()
{
    Settings::setDefaults();

    QSettings settings;

    auto *client = new Client();
    client->setServerAuthentication(settings.value("serverAuthentication").toString());
    client->setServerService(settings.value("serverService").toString());
    client->setServerStorage(settings.value("serverStorage").toString());
//    client->setUsername(settings.value("username").toString());
//    client->setPassword(settings.value("password").toString());

    return client;
}

int main(int argc, char **argv)
{
    QApplication::setOrganizationName("nehlsen");
    QApplication::setOrganizationDomain("nehlsen.org");
    QApplication::setApplicationName("VenueShotClient2");

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

    MainWindow mainWindow;
//    mainWindow.showFullScreen();
    mainWindow.show();

    int exitCode = app.exec();
    Logger::destroyInstance();
    return exitCode;
}
