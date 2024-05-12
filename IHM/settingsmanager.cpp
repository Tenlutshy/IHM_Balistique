#include "settingsmanager.h"

SettingsManager::SettingsManager() {}

QString SettingsManager::port;

void SettingsManager::loadSettings(){
    QDir settingsPath;
    QSettings settings(settingsPath.currentPath()+"/settings.ini", QSettings::IniFormat);

    port = settings.value("Port", "8000").toString();
}

void SettingsManager::writeSetting(QString key, QVariant variant)
{
    QDir settingsPath;
    QSettings settings(settingsPath.currentPath()+"/settings.ini", QSettings::IniFormat);

    settings.setValue(key, variant);
}
