#ifndef CLIENTTABLEMODEL_H
#define CLIENTTABLEMODEL_H

#include <QAbstractTableModel>

struct dataModel
{
    dataModel(QString _clientName, QString _ip)
    {
        clientName = _clientName;
        ip = _ip;
    }
    QString clientName;
    QString ip;
    QString chat;
};

class ClientTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ClientTableModel(QObject *parent = Q_NULLPTR);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void add(dataModel value);
    void update(const QModelIndex &index, dataModel value);
    void remove(const QModelIndex &index);

    const QList<dataModel> &getClientData();

private:
    QList<dataModel> values;
};

#endif // CLIENTTABLEMODEL_H
