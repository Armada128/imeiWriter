#include "baudmodel.h"

baudModel::baudModel(QObject *parent)
    : QAbstractListModel{parent}, m_bauds({1200, 2400, 4800,
               9600, 19200, 38400, 57600, 115200}), m_settings(MySettings::getInstance())
{
    m_baud = m_settings->getValue("BAUD", DEFAULT_BAUD).toInt();
}

int baudModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_bauds.count();
}

QVariant baudModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    if (row < 0 || row >= m_bauds.count()) return QVariant();
    return m_bauds.at(row);
}

int baudModel::baud() const {
    return m_baud;
}

void baudModel::onBaudSelected(int currentIndex, const QVariant& currentBaud) {
    int currentbaud = currentBaud.toInt();
    if(m_bauds.at(currentIndex) == currentbaud) {
        m_baud = currentbaud;
        qDebug() << "baud selected: " << m_baud;
        emit baudSelect(m_baud);
        m_settings->setValue("BAUD", m_baud);
    }
    else {
        qDebug() << "no matching baud";
    }

}




