#include "databitsmodel.h"

databitsModel::databitsModel(QObject *parent)
    : QAbstractListModel{parent}, m_databitss({5, 6, 7, 8}), m_settings(MySettings::getInstance())
{
    m_databits = m_settings->getValue("DATABITS", DEFAULT_DATABITS).toInt();
}

int databitsModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_databitss.count();
}

QVariant databitsModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    if (row < 0 || row >= m_databitss.count()) return QVariant();
    return m_databitss.at(row);
}

int databitsModel::databits() const {
    return m_databits;
}

void databitsModel::onDatabitsSelected(int currentIndex, const QVariant& currentDatabits) {
    int currentdatabits = currentDatabits.toInt();
    if(m_databitss.at(currentIndex) == currentdatabits) {
        m_databits = currentdatabits;
        qDebug() << "databits selected: " << m_databits;
        emit databitsSelect(m_databits);
        m_settings->setValue("DATABITS", m_databits);
    }
    else {
        qDebug() << "no matching databits";
    }

}
