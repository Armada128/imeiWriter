#include "myserial.h"

MySerial::MySerial(QObject* parent) : QObject(parent), m_serial(new QSerialPort(this))
{


    QObject::connect(m_serial.data(), &QSerialPort::errorOccurred, this, &MySerial::handleError);
    QObject::connect(m_serial.data(), &QSerialPort::readyRead, this, &MySerial::readData);
}

void MySerial::onComChanged(QString com) { m_com = com; }

void MySerial::onParityChanged(int parity) {
    switch (parity) {
    case 0:
        m_parity = QSerialPort::NoParity;
        break;
    case 2:
        m_parity = QSerialPort::EvenParity;
        break;
    case 3:
        m_parity = QSerialPort::OddParity;
    default:
        break;
    }
}

void MySerial::onBaudChanged(int baud) {
    switch (baud) {
    case 1200:
        m_baud = QSerialPort::Baud1200;
        break;
    case 2400:
        m_baud = QSerialPort::Baud2400;
        break;
    case 4800:
        m_baud = QSerialPort::Baud4800;
        break;
    case 9600:
        m_baud = QSerialPort::Baud9600;
        break;
    case 19200:
        m_baud = QSerialPort::Baud19200;
        break;
    case 38400:
        m_baud = QSerialPort::Baud38400;
        break;
    case 57600:
        m_baud = QSerialPort::Baud57600;
        break;
    case 115200:
        m_baud = QSerialPort::Baud115200;
        break;
    default:
        break;
    }
}

void MySerial::onDatabitsChanged(int databits) {
    switch (databits) {
    case 5:
        m_dataBits = QSerialPort::Data5;
        break;
    case 6:
        m_dataBits = QSerialPort::Data6;
        break;
    case 7:
        m_dataBits = QSerialPort::Data7;
        break;
    case 8:
        m_dataBits = QSerialPort::Data8;
        break;
    default:
        break;
    }
}

void MySerial::onStopbitsChanged(int stopbits) {
    switch (stopbits) {
    case 1:
        m_stopBits = QSerialPort::OneStop;
        break;
    case 2:
        m_stopBits = QSerialPort::TwoStop;
        break;
    default:
        break;
    }
}

void MySerial::onFlowcontrolChanged(int flowcontrol) {
    switch (flowcontrol) {
    case 0:
        m_flowcontrol = QSerialPort::NoFlowControl;
        break;
    case 1:
        m_flowcontrol = QSerialPort::HardwareControl;
        break;
    case 2:
        m_flowcontrol = QSerialPort::SoftwareControl;
        break;
    default:
        break;
    }
}

void MySerial::onConnect() {
    m_serial->setPortName(m_com);
    m_serial->setBaudRate(m_baud);
    m_serial->setDataBits(m_dataBits);
    m_serial->setParity(m_parity);
    m_serial->setStopBits(m_stopBits);
    m_serial->setFlowControl(m_flowcontrol);
    if (m_serial->open(QIODevice::ReadWrite)) {
        qDebug() << "serial " << m_com << " connected";
        emit connected();
    } else {
        qDebug() << "serial " << m_com << " failed to connect";
    }
}

void MySerial::onDisconnect() {
    if (m_serial->isOpen()) m_serial->close();
    qDebug() << "serial " << m_com << " disconnected";
    emit disconnected();
}

void MySerial::readData() {
    const QByteArray data = m_serial->readAll();
    if(data.size()) {
        m_buffer.append(data);
        QString all{m_buffer};
        auto packets = all.split(m_terminator);
        if (packets.size() > 1) {
            m_buffer.clear();
            m_buffer.append(packets.at(packets.size() - 1).toLocal8Bit());
            for (int i = 0; i < packets.size() - 1; ++i) {
                auto parts = packets.at(i).split(m_seperator);
                if (parts.size() == 2) {
                    if (parts.at(0) == "B") emit imeiRequired(parts.at(1));
                    else if (parts.at(0) == "C") emit idWritten();
                }
            }
        }
    }
}

void MySerial::onRequireIMEI() {
    auto written = m_serial->write(requireIMEI);
//    emit imeiReq(written == requireIMEI.size());
}

void MySerial::onWriteId(const QString& id) {
    QByteArray writeid = "C:";
    writeid.append(id.toLocal8Bit()).append("\r\n");
    auto len = m_serial->write(writeid);
    if (len == writeid.size()) emit idWritten();
    else qDebug() << "failed to send id";
}

void MySerial::handleError(QSerialPort::SerialPortError error) {
    qDebug() << error;
}











