#ifndef DATABITSMODEL_H
#define DATABITSMODEL_H

#include <QAbstractListModel>

#include "mysettings.h"
#include "myconstants.h"

class databitsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int databits READ databits NOTIFY databitsSelect)

public:
    explicit databitsModel(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& parent, int role) const override;
    int databits() const;

private:
    QList<int> m_databitss;
    int m_databits;
    QScopedPointer<MySettings> m_settings;

signals:
    void databitsSelect(int databits);

public slots:
    void onDatabitsSelected(int currentIndex, const QVariant& currentDatabits);
};

#endif // DATABITSMODEL_H
