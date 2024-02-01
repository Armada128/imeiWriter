#ifndef MYCONSTANTS_H
#define MYCONSTANTS_H

#include <QSerialPort>

constexpr int DEFAULT_BAUD = QSerialPort::Baud115200;
constexpr int DEFAULT_PARITY = QSerialPort::NoParity;
constexpr int DEFAULT_DATABITS = QSerialPort::Data8;
constexpr int DEFAULT_STOPBITS = QSerialPort::StopBits::OneStop;
constexpr int DEFAULT_FLOWCONTROL = QSerialPort::NoFlowControl;

#endif // MYCONSTANTS_H
