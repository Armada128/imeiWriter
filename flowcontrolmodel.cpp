#include "flowcontrolmodel.h"

flowcontrolModel::flowcontrolModel(QObject *parent)
    : QAbstractListModel{parent}, m_flowcontrols{{"No Flowcontrol", QSerialPort::NoFlowControl}, {"Hardware Flowcontrol", QSerialPort::HardwareControl},
                     {"Software Flowcontrol", QSerialPort::SoftwareControl}}, m_settings(MySettings::getInstance())
{
    m_rolenames[flowcontrolModel::flowcontrolType] = "type";
    m_rolenames[flowcontrolModel::flowcontrolValue] = "value";
    m_flowcontrol = m_settings->getValue("FLOWCONTROL", DEFAULT_FLOWCONTROL).toInt();
}

int flowcontrolModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_flowcontrols.count();
}

QVariant flowcontrolModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    if (row < 0 || row >= m_flowcontrols.count()) return QVariant();
    switch (role) {
    case flowcontrolModel::flowcontrolType:
        return std::get<0>(m_flowcontrols.at(row));
        break;
    case flowcontrolModel::flowcontrolValue:
        return std::get<1>(m_flowcontrols.at(row));
    default:
        break;
    }
    return QVariant();
}

QHash<int, QByteArray> flowcontrolModel::roleNames() const {
    return m_rolenames;
}

int flowcontrolModel::flowcontrol() const {
    return m_flowcontrol;
}

void flowcontrolModel::onFlowcontrolSelected(int currentIndex, const QString& currentFlowcontrol) {
    if(!std::get<0>(m_flowcontrols.at(currentIndex)).compare(currentFlowcontrol)) {
        m_flowcontrol = std::get<1>(m_flowcontrols.at(currentIndex));
        qDebug() << "flowcontrol selected: " << m_flowcontrol;
        emit flowcontrolSelect(m_flowcontrol);
    }
    else {
        qDebug() << "no matching flowcontrol";
    }
}
