#ifndef FLOWCONTROLMODEL_H
#define FLOWCONTROLMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QSerialPort>

#include "mysettings.h"
#include "myconstants.h"

class flowcontrolModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int flowcontrol READ flowcontrol NOTIFY flowcontrolSelect)

public:
    explicit flowcontrolModel(QObject *parent = nullptr);
    enum RoleNames {
        flowcontrolType = Qt::UserRole,
        flowcontrolValue = Qt::UserRole + 2
    };
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& parent, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    int flowcontrol() const;

private:
    const QList<std::tuple<QString, int>> m_flowcontrols;
    int m_flowcontrol;
    QHash<int, QByteArray> m_rolenames;
    QScopedPointer<MySettings> m_settings;

signals:
    void flowcontrolSelect(int flowcontrol);

public slots:
    void onFlowcontrolSelected(int currentIndex, const QString& currentFlowcontrol);
};

#endif // FLOWCONTROLMODEL_H
