#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>
#include <QSettings>
#include <QDir>

class SettingsManager
{
public:
    SettingsManager();

    static void loadSettings();
    static void writeSetting(QString key, QVariant variant);

    static QString port;
};

#endif // SETTINGSMANAGER_H
