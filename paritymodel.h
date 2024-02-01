#ifndef PARITYMODEL_H
#define PARITYMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QSerialPort>
#include <tuple>

#include "mysettings.h"
#include "myconstants.h"

class parityModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int parity READ parity NOTIFY paritySelect)
public:
    explicit parityModel(QObject *parent = nullptr);
    enum RoleNames {
        parityName = Qt::UserRole,
        parityValue = Qt::UserRole + 2
    };
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& parent, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    int parity() const;

private:
    const QList<std::tuple<QString, int>> m_parities;
    int m_parity;
    QHash<int, QByteArray> m_rolenames;
    QScopedPointer<MySettings> m_settings;

signals:
    void paritySelect(int parity);

public slots:
    void onParitySelected(int currentIndex, const QString& currentParity);
};

#endif // PARITYMODEL_H
