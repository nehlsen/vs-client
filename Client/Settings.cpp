#include <QtCore/QDir>
#include <QtCore/QSettings>
#include "Settings.h"

void Settings::setDefaults()
{
    QSettings settings;

    if (!settings.contains("serverService")) {
        settings.setValue("serverService", "http://api.my-server.com");
    }
    if (!settings.contains("serverStorage")) {
        settings.setValue("serverStorage", "http://storage.my-server.com");
    }
    if (!settings.contains("username")) {
        settings.setValue("username", "my-username");
    }
    if (!settings.contains("password")) {
        settings.setValue("password", "my-password");
    }
    if (!settings.contains("venue")) {
        settings.setValue("venue", "3");
    }
    
    if (!settings.contains("cacheFolder")) {
        settings.setValue("cacheFolder", QDir::homePath() + "/VenueShot/Cache");
    }
    
    if (!settings.contains("autoFetchEnabled")) {
        settings.setValue("autoFetchEnabled", true);
    }
    if (!settings.contains("autoUpdateInterval")) {
        settings.setValue("autoUpdateInterval", 10);
    }

    if (!settings.contains("slideShowInterval")) {
        settings.setValue("slideShowInterval", 1000);
    }

    settings.sync();
}
