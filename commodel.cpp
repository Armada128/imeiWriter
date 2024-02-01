#include "commodel.h"

comModel::comModel(QObject *parent)
    : QAbstractListModel{parent}
{
    refresh();
}

int comModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_ports.count();
}

QVariant comModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    if (row < 0 || row >= m_ports.count()) return QVariant();
    return m_ports.at(row);
}

QString comModel::com() const {
    return m_com;
}

void comModel::refresh() {
    int row = m_ports.count();
    if(row > 0)
    {
        beginRemoveRows(QModelIndex(), 0, row - 1);
        m_ports.clear();
        endRemoveRows();
    }
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    row = infos.count();
    beginInsertRows(QModelIndex(), 0, row - 1);
    for(const QSerialPortInfo& info : infos) {
        m_ports.append(info.portName());
//        qDebug() << "port: " << info.portName();
    }
    qDebug() << "port num: " << m_ports.size();
    endInsertRows();
}

void comModel::onComSelected(int currentIndex, const QVariant& currentCom) {
    QString currentcom = currentCom.toString();
    if(!m_ports.at(currentIndex).compare(currentcom)) {
        m_com = currentcom;
        qDebug() << "com selected: " << m_com;
        emit comSelect(m_com);
    }
    else {
        qDebug() << "no matching com";
    }
}
