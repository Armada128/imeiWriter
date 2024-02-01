#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QObject>
#include <QSettings>

class MySettings : public QObject
{
    Q_OBJECT

public:
    static MySettings* getInstance();

private:
    explicit MySettings(QObject *parent = nullptr);
    static MySettings* myself;
    QSettings m_settings;

signals:

public slots:
    void setValue(const QString& key, const QVariant& value);
    QVariant getValue(const QString& key, const QVariant& defaultValue = QVariant());
};

#endif // MYSETTINGS_H
