#ifndef MYSERIAL_H
#define MYSERIAL_H

#include <QObject>
#include <QSerialPort>
//#include <QTimer>
#include <QRegularExpression>
#include <cmath>

class MySerial : public QObject
{
    Q_OBJECT

public:
    explicit MySerial(QObject* parent = nullptr);

private:
    QString m_com;
    qint32 m_baud = 115200;
    QSerialPort::Parity m_parity = QSerialPort::EvenParity;
    QSerialPort::DataBits m_dataBits = QSerialPort::Data8;
    QSerialPort::StopBits m_stopBits = QSerialPort::OneStop;
    QSerialPort::FlowControl m_flowcontrol = QSerialPort::NoFlowControl;
    QScopedPointer<QSerialPort> m_serial;
    const QByteArray requireIMEI{"B:?\r\n"};
    QRegularExpression m_seperator{":"};
    QRegularExpression m_terminator{"\r\n"};
    QByteArray m_buffer;

    void readData();

signals:
    void connected();
    void disconnected();
    void imeiRequired(QString imei);
    void idWritten();

public slots:
    void onConnect();
    void onDisconnect();
    void onComChanged(QString com);
    void onParityChanged(int parity);
    void onBaudChanged(int baud);
    void onDatabitsChanged(int databits);
    void onStopbitsChanged(int stopbits);
    void onFlowcontrolChanged(int flowcontrol);
    void onRequireIMEI();
    void onWriteId(const QString& id);
    void handleError(QSerialPort::SerialPortError error);
};

#endif // MYSERIAL_H
