#include "stopbitsmodel.h"

stopbitsModel::stopbitsModel(QObject *parent)
    : QAbstractListModel{parent}, m_stopbitss({1, 2}), m_settings(MySettings::getInstance())
{
    m_stopbits = m_settings->getValue("STOPBITS", DEFAULT_STOPBITS).toInt();
}

int stopbitsModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_stopbitss.count();
}

QVariant stopbitsModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    if (row < 0 || row >= m_stopbitss.count()) return QVariant();
    return m_stopbitss.at(row);
}

int stopbitsModel::stopbits() const {
    return m_stopbits;
}

void stopbitsModel::onStopbitsSelected(int currentIndex, const QVariant& currentStopbits) {
    int currentstopbits = currentStopbits.toInt();
    if(m_stopbitss.at(currentIndex) == currentstopbits) {
        m_stopbits = currentstopbits;
        qDebug() << "stopbits selected: " << m_stopbits;
        emit stopbitsSelect(m_stopbits);
        m_settings->setValue("STOPBITS", m_stopbits);
    }
    else {
        qDebug() << "no matching stopbits";
    }

}
