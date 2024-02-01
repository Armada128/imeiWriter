#ifndef SERIALMOVETOTHREAD_H
#define SERIALMOVETOTHREAD_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QRegularExpression>
#include <QQmlListProperty>

#include "myserial.h"
#include "myfile.h"
#include "devicemodel.h"

class SerialMoveToThread : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int conn READ conn NOTIFY connChanged)
    Q_PROPERTY(QString imei READ imei NOTIFY imeiChanged)
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(bool fileok READ fileok NOTIFY fileokChanged)
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(DeviceModel* model READ model CONSTANT)

public:
    enum Connection {Connected, Disconnected};
    Q_ENUM(Connection);
    enum Field {ID, VERSION};
    Q_ENUM(Field);
    explicit SerialMoveToThread(QObject *parent = nullptr);
    ~SerialMoveToThread() noexcept;

    int conn();
    QString imei();
    QString id();
    void setId(const QString& id);
    bool fileok();
    QString user();
    void setUser(const QString& user);
    QString version();
    void setVersion(const QString& version);
    DeviceModel* model() { return &m_model; }

private:
    QThread serialThread{this};
    QThread fileThread{this};
    int m_conn{Disconnected};
    MySerial m_serial;
    MyFile m_file;
    DeviceModel m_model;
    QTimer m_timer{this};
    QString m_imei;
    QString m_id;
    bool m_fileok{true};
    QString m_user;
    QString m_version;
    QString m_prefix{"Nan"};
    QRegularExpression reg_id{"^[A-Z]\\d{5}[A-Z0-9]{5}$"};
    QRegularExpression reg_version{"^([1-9](\\d)*){1,}((\\.[1-9](\\d)*)|(\\.0))*$"};
//    QRegularExpression reg_prefix{"^[a-zA-Z]\\d{5}$"};

signals:
    void connChanged(int conn);
    void connectSerial();
    void disconnectSerial();
    void comChanged(const QString& com);
    void parityChanged(int parity);
    void baudChanged(int baud);
    void databitsChanged(int databits);
    void stopbitsChanged(int stopbits);
    void flowcontrolChanged(int flowcontrol);
    void blankImei();
    void imeiChanged(const QString& imei);
    void idChanged(const QString& id);
    void writeId(const QString& id);
    void confirmId(const QString& id, const QString& imei, const QString& user, const QString& version);
    void idWritten();
    void fileokChanged(bool fileok);
    void recordExists(QString imei);
    void recorded();
    void makeRecord(const QString& id, const QString& imei, const QString& user, const QString& version);
    void stopFileTimer();
    void userChanged(const QString& user);
    void versionChanged(const QString& user);
    void idValid(bool valid);
    void versionValid(bool valid);
    void selectPrefix(const QString& prefix);
    void prefixValid(bool valid);
    void saveDevices(const QString& dir);
    void clearModel();
    void deviceSelected(const int index, bool op);
    void fileChanged(const QString& path);

public slots:
    void onConnect();
    void onDisconnect();
    void onCom(const QString& com);
    void onParity(int parity);
    void onBaud(int baud);
    void onDatabits(int databits);
    void onStopbits(int stopbits);
    void onFlowcontrol(int flowcontrol);
    void onWriteId(const QString& id);
    void onConfirmId(const QString& id);
    void onRefresh();
    void checkValid(Field field, const QString& content);
//    void checkPrefix(const QString& prefix);
    void onSelectPrefix(const QString& prefix);
    void onSaveDevices(const QString& dir);
    void onSelected(const int index, bool op);

private slots:
    void onConnected();
    void onDisconnected();
    void onImeiRequired(QString imei);
    void onRecordExists(QString imei);
    void onFileStatus(bool flag);
    void onIdWritten();
    void onFileChanged(const QString& path);
};

#endif // SERIALMOVETOTHREAD_H
