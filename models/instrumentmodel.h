#ifndef DAFELAB_INSTRUMENTMODEL_H
#define DAFELAB_INSTRUMENTMODEL_H
#include <cmath>

#include <QAbstractTableModel>
#include "../logic/manager.h"

class Manager;

class InstrumentModel : public QAbstractTableModel
{
public:
    explicit InstrumentModel() = default;
private:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};
//    QModelIndex parent(const QModelIndex &child) const override { return {}; };



#endif // INSTRUMENTMODEL_H
