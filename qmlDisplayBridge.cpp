#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>
#include <Client/Client.h>
#include <Client/Widget/SlideShowWidget.h>
#include <Client/VenueShot.h>
#include "QsLog/QsLog.h"

using namespace QsLogging;

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
//    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);
//    logger.addDestination(functorDestination);

    // ------------------------------------------------------------------------------------------------

    VenueShot::declareQml();

    QQuickView mainView;
    mainView.setSource(QUrl::fromLocalFile("qml/displayBridge.qml"));
//    mainView.showFullScreen();
    mainView.show();

    int exitCode = app.exec();
    Logger::destroyInstance();
    return exitCode;
}
