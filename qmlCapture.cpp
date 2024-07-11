#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>
#include <Client/Client.h>
#include <Client/Widget/SlideShowWidget.h>
#include <Client/VenueShot.h>
#include <QtCore/QTimer>
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"

#ifndef VSC_FULLSCREEN
#define VSC_FULLSCREEN false
#endif

using namespace QsLogging;

int main(int argc, char **argv)
{
    QApplication::setOrganizationName("nehlsen");
    QApplication::setOrganizationDomain("nehlsen.org");
    QApplication::setApplicationName("VenueShotClient2");

    QApplication app(argc, argv);

    Logger& logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(app.applicationDirPath()).filePath("vsc.log"));

    DestinationPtrU fileDestination(DestinationFactory::MakeFileDestination(
            sLogPath, LogRotationOption::EnableLogRotation, MaxSizeBytes(1024*1024), MaxOldLogCount(2)));
    logger.addDestination(std::move(fileDestination));

#ifdef VSC_CLI_LOG
    DestinationPtrU debugDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(std::move(debugDestination));
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

    return app.exec();
}
