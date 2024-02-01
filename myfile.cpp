#include "myfile.h"

MyFile::MyFile(QObject *parent)
    : QObject{parent}
{
    QString dir_complete = QDir::homePath().append("/imei.json");
    QString dir = QDir::toNativeSeparators(dir_complete);
    QFileInfo info{dir};
    m_file = QSharedPointer<QFile>(new QFile{dir});
    qDebug() << "file dir: " << info.absoluteFilePath();
    m_timer = QSharedPointer<QTimer>(new QTimer{this});
    m_watcher.addPath(m_file->fileName());
    QObject::connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &MyFile::onFileChanged);
}

MyFile::~MyFile() noexcept {}

void MyFile::onStarted() {
    m_timer = QSharedPointer<QTimer>(new QTimer(this));
    QObject::connect(m_timer.get(), &QTimer::timeout, this, &MyFile::checkFile);
    m_timer->start(1000);
}

void MyFile::checkFile() {
    if (!m_file->exists()) emit fileStatus(false);
    else emit fileStatus(true);
}

void MyFile::onRecord(const QString& id, const QString& imei, const QString& user, const QString& version) {
    m_file->open(QIODevice::ReadOnly);
    QByteArray jsonData = m_file->readAll();
    m_file->close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isObject()) {
        QJsonObject obj = jsonDoc.object();
        auto v = obj.value(id);
        if (v.isUndefined()) {
            qDebug() << id << " undefined";
            QJsonObject subobj;
            subobj.insert("TIME", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz dddd"));
            subobj.insert("USER", user);
            subobj.insert("VERSION", version);
            subobj.insert("IMEI", imei);
            subobj.insert("ID_SHORT", id.left(1).append("-").append(id.right(5)));
            obj.insert(id, subobj);
            QByteArray tobewritten = QJsonDocument{obj}.toJson();
            qDebug() << tobewritten;
            m_file->open(QIODevice::ReadWrite | QIODeviceBase::Truncate);
            auto len = m_file->write(tobewritten);
            m_file->close();
            if (len == tobewritten.size()) emit recorded();
            else qDebug() << "write failed";
        } else {
            auto subobj_temp = v.toObject();
            auto imei_temp = subobj_temp.value("IMEI").toString();
            qDebug() << "id exists with imei: " << imei_temp;
            if (imei_temp != imei) emit recordExists(imei_temp);
        }
    } else {
        qDebug() << "file is not a json";
        QJsonObject obj;        
        QJsonObject subobj;
        subobj.insert("TIME", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz dddd"));
        subobj.insert("USER", user);
        subobj.insert("VERSION", version);
        subobj.insert("IMEI", imei);
        subobj.insert("ID_SHORT", id.left(1).append("-").append(id.right(5)));
        obj.insert(id, subobj);
        QByteArray tobewritten = QJsonDocument{obj}.toJson();
        m_file->open(QIODevice::ReadWrite | QIODeviceBase::Truncate);
        auto len = m_file->write(tobewritten);
        m_file->close();
        if (len == tobewritten.size()) emit recorded();
        else qDebug() << "write failed";
    }
}

void MyFile::onConfirm(const QString& id, const QString& imei, const QString& user, const QString& version) {
    m_file->open(QIODevice::ReadOnly);
    QByteArray jsonData = m_file->readAll();
    m_file->close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    QJsonObject obj = jsonDoc.object();
    auto subobjref = obj[id];
    QJsonObject subobj;
    subobj.insert("TIME", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz dddd"));
    subobj.insert("USER", user);
    subobj.insert("VERSION", version);
    subobj.insert("IMEI", imei);
    subobj.insert("ID_SHORT", id.left(1).append("-").append(id.right(5)));
    subobjref = subobj;
    QByteArray tobewritten = QJsonDocument{obj}.toJson();
    m_file->open(QIODevice::ReadWrite | QIODeviceBase::Truncate);
    auto len = m_file->write(tobewritten);
    m_file->close();
    if (len == tobewritten.size()) emit recorded();
    else qDebug() << "write failed";
}

void MyFile::onStopTimer() {
    m_timer->stop();
    qDebug() << "file timer stopped";
}

void MyFile::onSelectPrefix(const QString& prefix) {
    bool nonemptyPrefix = !prefix.isEmpty();
    auto prefixSize = prefix.size();
    if (m_model) {
        m_file->open(QIODevice::ReadOnly);
        QByteArray jsonData = m_file->readAll();
        m_file->close();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (jsonDoc.isObject()) {
            QJsonObject obj = jsonDoc.object();
            QStringList keys = obj.keys();
            for (int i = 0; i < keys.size(); ++i) {
                QString key = keys.at(i);
                auto prefix_temp = key.left(prefixSize);
                auto prefix_match = (prefix_temp != prefix);
                qDebug() << "key prefix: " << prefix_temp << ", match res: " << prefix_match;
                if (nonemptyPrefix && prefix_match) continue;
                QJsonValue val = obj.value(key);
                if (val.isObject()) {
                    std::shared_ptr<DeviceModel::MyDevice> dm = std::make_shared<DeviceModel::MyDevice>();
                    dm->id = key;
                    QJsonObject sub_obj = val.toObject();
                    QStringList sub_keys = sub_obj.keys();
                    for (int j = 0; j < sub_keys.size(); ++j) {
                        QString sub_key = sub_keys.at(j);
                        if (sub_key == "IMEI") dm->imei = sub_obj.value(sub_key).toString();
                        else if (sub_key == "USER") dm->user = sub_obj.value(sub_key).toString();
                        else if (sub_key == "VERSION") dm->version = sub_obj.value(sub_key).toString();
                        else if (sub_key == "TIME") dm->time = sub_obj.value(sub_key).toString();
                    }
                    m_model->beginInsertRows(QModelIndex(), m_model->m_devices.size(), m_model->m_devices.size());
                    m_model->m_devices.append(dm);
                    m_model->endInsertRows();
                    qDebug() << "device: " << dm->id << " appended";
                }
            }
        }
    }
}

void MyFile::onSaveDevices(const QString& dir) {
    qDebug() << "saving devices to: " << dir;
    if (m_model->m_selected.isEmpty()) return;
    QJsonObject obj;
    QJsonArray arr;
    for (const auto& e : m_model->m_selected) {
        qDebug() << "device: " << m_model->m_devices.at(e)->id;
        arr.append(m_model->m_devices.at(e)->id);
    }
    obj.insert("DEVICES", arr);
    QJsonDocument doc(obj);
    QByteArray json = doc.toJson();
    QFile file_temp{dir};
    if (!file_temp.open(QIODevice::ReadWrite)) {
        emit deviceSaved(false);
        return;
    }
    file_temp.resize(0);
    file_temp.write(json);
    file_temp.close();
}

void MyFile::onFileChanged(const QString& path) {
    qDebug() << "changed path: " << path << ", m_file: " << m_file->fileName();
//    if (path != m_file->fileName()) emit fileChanged(path);
}
