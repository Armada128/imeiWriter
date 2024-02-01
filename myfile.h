#ifndef MYFILE_H
#define MYFILE_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileSystemWatcher>

#include "myconstants.h"
#include "devicemodel.h"

class MyFile : public QObject
{
    Q_OBJECT

public:
    explicit MyFile(QObject *parent = nullptr);
    ~MyFile() noexcept;
    void init(DeviceModel* model) { m_model = model; }

private:
    QSharedPointer<QFile> m_file{nullptr};
    QSharedPointer<QTimer> m_timer{nullptr};
    DeviceModel* m_model{nullptr};
    QString m_prefix;
    QFileSystemWatcher m_watcher;

    void checkFile();

signals:
    void fileStatus(bool flag);
    void recordExists(QString imei);
    void recorded();
    void deviceSaved(bool saved);
    void fileChanged(const QString& path);

public slots:
    void onStarted();
    void onRecord(const QString& id, const QString& imei, const QString& user, const QString& version);
    void onConfirm(const QString& id, const QString& imei, const QString& user, const QString& version);
    void onStopTimer();
    void onSelectPrefix(const QString& prefix);
    void onSaveDevices(const QString& dir);

private slots:
    void onFileChanged(const QString& path);
};

#endif // MYFILE_H
