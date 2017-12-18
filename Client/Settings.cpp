#include <QtCore/QDir>
#include <QtCore/QSettings>
#include "Settings.h"

void Settings::setDefaults()
{
    QSettings settings;

    settings.beginGroup("Server");
    if (!settings.contains("Url")) {
        settings.setValue("Url", "http://vesh.my-server.com");
    }
    if (!settings.contains("Username")) {
        settings.setValue("Username", "my-username");
    }
    if (!settings.contains("Password")) {
        settings.setValue("Password", "my-password");
    }
    if (!settings.contains("Venue")) {
        settings.setValue("Venue", "3");
    }
    settings.endGroup();

    settings.beginGroup("Local");
    if (!settings.contains("CacheFolder")) {
        settings.setValue("CacheFolder", QDir::homePath() + "/VenueShot/Cache");
    }
    settings.endGroup();

    settings.beginGroup("Config");
    if (!settings.contains("AutoFetchEnabled")) {
        settings.setValue("AutoFetchEnabled", true);
    }
    if (!settings.contains("AutoUpdateInterval")) {
        settings.setValue("AutoUpdateInterval", 10);
    }
    settings.endGroup();

    settings.beginGroup("Display");
    if (!settings.contains("AdvancePictureDelay")) {
        settings.setValue("AdvancePictureDelay", 1);
    }
    settings.endGroup();

    settings.sync();
}
