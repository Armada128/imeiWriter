#include "serialmovetothread.h"

SerialMoveToThread::SerialMoveToThread(QObject *parent)
    : QObject{parent}
{
    m_timer.setInterval(1000);
    m_serial.moveToThread(&serialThread);
    m_file.init(&m_model);
    m_file.moveToThread(&fileThread);
    QObject::connect(this, &SerialMoveToThread::connectSerial, &m_serial, &MySerial::onConnect);
    QObject::connect(this, &SerialMoveToThread::disconnectSerial, &m_serial, &MySerial::onDisconnect);
    QObject::connect(this, &SerialMoveToThread::comChanged, &m_serial, &MySerial::onComChanged);
    QObject::connect(this, &SerialMoveToThread::parityChanged, &m_serial, &MySerial::onParityChanged);
    QObject::connect(this, &SerialMoveToThread::baudChanged, &m_serial, &MySerial::onBaudChanged);
    QObject::connect(this, &SerialMoveToThread::databitsChanged, &m_serial, &MySerial::onDatabitsChanged);
    QObject::connect(this, &SerialMoveToThread::stopbitsChanged, &m_serial, &MySerial::onStopbitsChanged);
    QObject::connect(this, &SerialMoveToThread::flowcontrolChanged, &m_serial, &MySerial::onFlowcontrolChanged);
    QObject::connect(this, &SerialMoveToThread::writeId, &m_serial, &MySerial::onWriteId);
    QObject::connect(this, &SerialMoveToThread::confirmId, &m_file, &MyFile::onConfirm);
    QObject::connect(this, &SerialMoveToThread::makeRecord, &m_file, &MyFile::onRecord);
    QObject::connect(this, &SerialMoveToThread::stopFileTimer, &m_file, &MyFile::onStopTimer);
    QObject::connect(this, &SerialMoveToThread::selectPrefix, &m_file, &MyFile::onSelectPrefix);
    QObject::connect(this, &SerialMoveToThread::saveDevices, &m_file, &MyFile::onSaveDevices);
    QObject::connect(this, &SerialMoveToThread::clearModel, &m_model, &DeviceModel::onClear);
    QObject::connect(this, &SerialMoveToThread::deviceSelected, &m_model, &DeviceModel::onSelected);
    QObject::connect(&m_serial, &MySerial::connected, this, &SerialMoveToThread::onConnected);
    QObject::connect(&m_serial, &MySerial::disconnected, this, &SerialMoveToThread::onDisconnected);
    QObject::connect(&m_serial, &MySerial::imeiRequired, this, &SerialMoveToThread::onImeiRequired);
    QObject::connect(&m_serial, &MySerial::idWritten, this, &SerialMoveToThread::onIdWritten);
    QObject::connect(&m_file, &MyFile::fileStatus, this, &SerialMoveToThread::onFileStatus);
    QObject::connect(&m_file, &MyFile::recordExists, this, &SerialMoveToThread::onRecordExists);
    QObject::connect(&m_file, &MyFile::recorded, this, [this]{emit recorded();});
    QObject::connect(&m_file, &MyFile::fileChanged, this, &SerialMoveToThread::onFileChanged);
    QObject::connect(&fileThread, &QThread::started, &m_file, &MyFile::onStarted);
    QObject::connect(&m_timer, &QTimer::timeout, &m_serial, &MySerial::onRequireIMEI);
    serialThread.start();
    fileThread.start();
}

SerialMoveToThread::~SerialMoveToThread() noexcept {
    emit stopFileTimer();
    qDebug() << "ready to stop file timer";
    serialThread.quit();
    fileThread.quit();
    serialThread.wait(1000);
    fileThread.wait(1000);
}

int SerialMoveToThread::conn() {
    return m_conn;
}

QString SerialMoveToThread::imei() {
    return m_imei;
}

QString SerialMoveToThread::id() {
    return m_id;
}

void SerialMoveToThread::setId(const QString& id) {
    m_id = id;
    emit idChanged(m_id);
}

bool SerialMoveToThread::fileok() {
    return m_fileok;
}

QString SerialMoveToThread::user() {
    return m_user;
}

void SerialMoveToThread::setUser(const QString& user) {
    m_user = user;
    emit userChanged(m_user);
}

QString SerialMoveToThread::version() {
    return m_version;
}

void SerialMoveToThread::setVersion(const QString& version) {
    m_version = version;
    emit versionChanged(m_version);
}

void SerialMoveToThread::onConnect() {
    emit connectSerial();
}

void SerialMoveToThread::onDisconnect() {
    emit disconnectSerial();
}

void SerialMoveToThread::onCom(const QString& com) {
    emit comChanged(com);
}

void SerialMoveToThread::onParity(int parity) {
    emit parityChanged(parity);
}

void SerialMoveToThread::onBaud(int baud) {
    emit baudChanged(baud);
}

void SerialMoveToThread::onDatabits(int databits) {
    emit databitsChanged(databits);
}

void SerialMoveToThread::onStopbits(int stopbits) {
    emit stopbitsChanged(stopbits);
}

void SerialMoveToThread::onFlowcontrol(int flowcontrol) {
    emit flowcontrolChanged(flowcontrol);
}

void SerialMoveToThread::onWriteId(const QString& id) {
    emit writeId(id);
}

void SerialMoveToThread::onConfirmId(const QString& id) {
    emit confirmId(id, m_imei, m_user, m_version);
}

void SerialMoveToThread::onRefresh() {
    m_imei.clear();
    emit blankImei();
    emit imeiChanged(m_imei);
    m_timer.start();
}

void SerialMoveToThread::checkValid(Field field, const QString& content) {
    if (field == ID) {
        if (reg_id.match(content).hasMatch()) {
            m_id = content;
            emit idChanged(m_id);
            emit idValid(true);
        } else {
            m_id.clear();
            emit idValid(false);
        }
    } else if (field == VERSION) {
        if (reg_version.match(content).hasMatch()) {
            m_version = content;
            emit versionChanged(m_version);
            emit versionValid(true);
        } else {
            m_version.clear();
            emit versionValid(false);
        }
    }
}
/*
void SerialMoveToThread::checkPrefix(const QString& prefix) {
    if (reg_prefix.match(prefix).hasMatch()) emit prefixValid(true);
    else emit prefixValid(false);
}
*/
void SerialMoveToThread::onSelectPrefix(const QString& prefix) {
    qDebug() << "prefix: " << prefix << " , m_prefix: " << m_prefix;
//    if (prefix != m_prefix) {
        m_prefix = prefix;
        emit clearModel();
        emit selectPrefix(prefix);
//    }
}

void SerialMoveToThread::onSaveDevices(const QString& dir) {
    emit saveDevices(dir);
}

void SerialMoveToThread::onSelected(const int index, bool op) {
    emit deviceSelected(index, op);
}

void SerialMoveToThread::onConnected() {
    m_conn = Connected;
    emit connChanged(m_conn);
    m_timer.start();
}

void SerialMoveToThread::onDisconnected() {
    m_conn = Disconnected;
    emit connChanged(m_conn);
    m_imei.clear();
    emit imeiChanged("");
    m_timer.stop();
}

void SerialMoveToThread::onImeiRequired(QString imei) {
    if (imei.isEmpty()) emit blankImei();
    else {
        m_timer.stop();
        m_imei = imei;
        emit imeiChanged(imei);
    }
}

void SerialMoveToThread::onRecordExists(QString imei) {
    qDebug() << "SerialMoveToThread::onRecordExists: " << imei << " exists";
    emit recordExists(imei);
}

void SerialMoveToThread::onFileStatus(bool flag) {
    m_fileok = flag;
    emit fileokChanged(m_fileok);
}

void SerialMoveToThread::onIdWritten() {
    emit idWritten();
    qDebug() << "making record: " << m_id << "," << m_imei << "," << m_user << "," << m_version;
    emit makeRecord(m_id, m_imei, m_user, m_version);
}

void SerialMoveToThread::onFileChanged(const QString& path) {
    emit fileChanged(path);
//    emit selectPrefix(m_prefix);
}

