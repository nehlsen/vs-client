#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <Client/Client.h>
#include <Client/Widget/MainWindow.h>
#include "QsLog/QsLog.h"

using namespace QsLogging;

Client* createClient()
{
    QSettings settings;
    settings.beginGroup("Server");
    if (!settings.contains("Url")) {
        settings.setValue("Url", "http://vesh.local");
    }
    if (!settings.contains("Username")) {
        settings.setValue("Username", "nehlsen");
    }
    if (!settings.contains("Password")) {
        settings.setValue("Password", "xc1337");
    }
    settings.endGroup();
    settings.sync();

    auto *client = new Client();
    client->setServer(settings.value("Server/Url").toString());
//    client->setUsername(settings.value("Server/Username").toString());
//    client->setPassword(settings.value("Server/Password").toString());

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

    MainWindow mainWindow;
//    mainWindow.showFullScreen();
    mainWindow.show();

    int exitCode = app.exec();
    Logger::destroyInstance();
    return exitCode;
}
