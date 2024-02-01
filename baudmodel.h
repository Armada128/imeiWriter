#ifndef BAUDMODEL_H
#define BAUDMODEL_H

#include <QAbstractListModel>

#include "mysettings.h"
#include "myconstants.h"

class baudModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int baud READ baud NOTIFY baudSelect)
public:
    explicit baudModel(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& parent, int role) const override;
    int baud() const;

private:
    QList<int> m_bauds;
    int m_baud;
    QScopedPointer<MySettings> m_settings;

signals:
    void baudSelect(int baud);

public slots:
    void onBaudSelected(int currentIndex, const QVariant& currentBaud);
};

#endif // BAUDMODEL_H
