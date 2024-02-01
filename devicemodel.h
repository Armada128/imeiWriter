#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QAbstractListModel>
#include <QObject>

class DeviceModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int groupState READ groupState NOTIFY groupStateChanged)

public:
    enum GroupStates {
        Unchecked,
        Partial,
        Checked
    };
    Q_ENUM(GroupStates)
    enum Rolenames {
        deviceId = Qt::UserRole,
        deviceImei = Qt::UserRole + 1,
        deviceUser = Qt::UserRole + 2,
        deviceVersion = Qt::UserRole + 3,
        deviceTime = Qt::UserRole + 4
    };
    explicit DeviceModel(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex& index) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    int groupState() { return m_groupstate; }

private:
    class MyDevice {
    public:
        QString id;
        QString imei;
        QString user;
        QString version;
        QString time;
    };
    QHash<int, QByteArray> m_rolenames;
    QList<std::shared_ptr<MyDevice>> m_devices;
    QSet<qsizetype> m_selected;
    int m_groupstate{Unchecked};
    friend class MyFile;

signals:
    void groupStateChanged(int state);

public slots:
    void onClear();
    void onSelected(const int index, bool op);
};

#endif // DEVICEMODEL_H
