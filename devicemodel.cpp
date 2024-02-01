#include "devicemodel.h"

DeviceModel::DeviceModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_rolenames[deviceId] = "id";
    m_rolenames[deviceImei] = "imei";
    m_rolenames[deviceUser] = "user";
    m_rolenames[deviceVersion] = "version";
    m_rolenames[deviceTime] = "time";
}

int DeviceModel::rowCount(const QModelIndex& index) const {
    Q_UNUSED(index);
    return m_devices.count();
}

QVariant DeviceModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    switch (role) {
    case deviceId:
        return m_devices.at(row)->id;
        break;
    case deviceImei:
        return m_devices.at(row)->imei;
        break;
    case deviceUser:
        return m_devices.at(row)->user;
        break;
    case deviceVersion:
        return m_devices.at(row)->version;
        break;
    case deviceTime:
        return m_devices.at(row)->time;
        break;
    default:
        return QVariant();
        break;
    }
}

QHash<int, QByteArray> DeviceModel::roleNames() const { return m_rolenames; }

void DeviceModel::onClear() {
    m_selected.clear();
    emit groupStateChanged(Unchecked);
    beginRemoveRows(QModelIndex(), 0, m_devices.size());
    m_devices.clear();
    endRemoveRows();
    qDebug() << "device list cleared";
}

void DeviceModel::onSelected(const int index, bool op) {
    if (op) {
        if (index >= 0 && index < m_devices.size()) {
            m_selected.insert(index);
            if (m_selected.size() == m_devices.size()) m_groupstate = Checked;
            else m_groupstate = Partial;
            emit groupStateChanged(m_groupstate);
        }
    } else if(m_selected.remove(index)) {
        if (m_selected.size() == 0) m_groupstate = Unchecked;
        else m_groupstate = Partial;
        emit groupStateChanged(m_groupstate);
    }
}
