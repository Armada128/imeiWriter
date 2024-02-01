#ifndef COMMODEL_H
#define COMMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QSerialPortInfo>

class comModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString com READ com NOTIFY comSelect)

public:
    explicit comModel(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& parent, int role) const override;
    QString com() const;

private:
    QList<QString> m_ports;
    QString m_com;

signals:
    void comSelect(QString com);

public slots:
    void refresh();
    void onComSelected(int currentIndex, const QVariant& currentCom);
};

#endif // COMMODEL_H
