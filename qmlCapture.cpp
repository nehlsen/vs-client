#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>
#include <Client/Client.h>
#include <Client/Widget/SlideShowWidget.h>
#include <Client/VenueShot.h>
#include <QtCore/QTimer>
#include "QsLog/QsLog.h"

#ifndef VSC_FULLSCREEN
#define VSC_FULLSCREEN false
#endif

using namespace QsLogging;

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

    VenueShot::declareQml();

    QQuickView mainView;
    mainView.setSource(QUrl::fromLocalFile("qml/capture.qml"));
    if (VSC_FULLSCREEN) {
        QTimer::singleShot(0, &mainView, &QQuickView::showFullScreen);
    } else {
        mainView.show();
    }

    int exitCode = app.exec();
    Logger::destroyInstance();
    return exitCode;
}
