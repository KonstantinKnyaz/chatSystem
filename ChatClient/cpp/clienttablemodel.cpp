#include "clienttablemodel.h"

ClientTableModel::ClientTableModel(QObject *parent) : QAbstractTableModel(parent)
{

}

int ClientTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return values.count();
}

int ClientTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant ClientTableModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    switch (role) {
    case Qt::DisplayRole:
    {
        switch (index.column()) {
        case 0: {
            value = this->values.at(index.row()).clientName;
            break;
        }
        case 1: {
            value = this->values.at(index.row()).ip;
            break;
        }
        }
    }
        break;
    default:
        return value;
    }
    return value;
}

QVariant ClientTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Ник");
        case 1:
            return QString("IP");
        }
    }
    return QVariant();
}

void ClientTableModel::add(dataModel value)
{
    int newRow = this->values.count();

    this->beginInsertRows(QModelIndex(), newRow, newRow);
    values.append(value);
    endInsertRows();
}

void ClientTableModel::update(const QModelIndex &index, dataModel value)
{
    values[index.row()] = value;
    emit this->dataChanged(index, index);
}

void ClientTableModel::remove(const QModelIndex &index)
{
    this->beginRemoveRows(QModelIndex(), index.row(), index.row());
    values.removeAt(index.row());
    this->endRemoveRows();
}

const QList<dataModel> &ClientTableModel::getClientData()
{
    return values;
}
