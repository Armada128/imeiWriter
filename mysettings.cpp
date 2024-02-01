#include "mysettings.h"
#define SETTINGSDIR "./settings.ini"

MySettings* MySettings::myself = new MySettings;

MySettings::MySettings(QObject *parent)
    : QObject{parent}, m_settings(SETTINGSDIR, QSettings::IniFormat)
{
    m_settings.sync();
}

MySettings* MySettings::getInstance() { return myself; }

void MySettings::setValue(const QString& key, const QVariant& value) {
    m_settings.setValue(key, value);
    m_settings.sync();
}

QVariant MySettings::getValue(const QString& key, const QVariant& defaultValue) {
    return m_settings.value(key, defaultValue);
}
