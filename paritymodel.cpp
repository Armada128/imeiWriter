#include "paritymodel.h"

parityModel::parityModel(QObject *parent)
    : QAbstractListModel{parent}, m_parities{{"No Parity", QSerialPort::NoParity}, {"Odd Parity", QSerialPort::OddParity},
                 {"Even Parity", QSerialPort::EvenParity}}, m_settings(MySettings::getInstance())
{
    m_rolenames[parityModel::parityName] = "name";
    m_rolenames[parityModel::parityValue] = "value";
    m_parity = m_settings->getValue("PARITY", DEFAULT_PARITY).toInt();
}

int parityModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_parities.count();
}

QVariant parityModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    if (row < 0 || row >= m_parities.count()) return QVariant();
    switch (role) {
    case parityModel::parityName:
        return std::get<0>(m_parities.at(row));
        break;
    case parityModel::parityValue:
        return std::get<1>(m_parities.at(row));
    default:
        break;
    }
    return QVariant();
}

QHash<int, QByteArray> parityModel::roleNames() const {
    return m_rolenames;
}

int parityModel::parity() const {
    return m_parity;
}

void parityModel::onParitySelected(int currentIndex, const QString& currentParity) {
    if(!std::get<0>(m_parities.at(currentIndex)).compare(currentParity)) {
        m_parity = std::get<1>(m_parities.at(currentIndex));
        qDebug() << "parity selected: " << m_parity;
        emit paritySelect(m_parity);
        m_settings->setValue("PARITY", m_parity);
    }
    else {
        qDebug() << "no matching parity";
    }
}
