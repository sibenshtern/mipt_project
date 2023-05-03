#pragma once

#include <cmath>

#include <QAbstractTableModel>
#include "../logic/manager.h"

class Manager;

class VisualModel : public QAbstractTableModel
{
public:
    explicit VisualModel() = default;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override { return {}; };
    Qt::ItemFlags flags(const QModelIndex &index) const override;
private:

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

//    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
//    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
//    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
//    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
//    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    int COLUMN_WITH_CHECKBOX {0};
};
//    QModelIndex parent(const QModelIndex &child) const override { return {}; };
