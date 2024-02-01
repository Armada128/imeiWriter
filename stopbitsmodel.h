#ifndef STOPBITSMODEL_H
#define STOPBITSMODEL_H

#include <QAbstractListModel>

#include "mysettings.h"
#include "myconstants.h"

class stopbitsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int stopbits READ stopbits NOTIFY stopbitsSelect)

public:
    explicit stopbitsModel(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& parent, int role) const override;
    int stopbits() const;

private:
    QList<int> m_stopbitss;
    int m_stopbits;
    QScopedPointer<MySettings> m_settings;

signals:
    void stopbitsSelect(int stopbits);

public slots:
    void onStopbitsSelected(int currentIndex, const QVariant& currentStopbits);
};

#endif // STOPBITSMODEL_H
