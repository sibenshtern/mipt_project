#pragma once

#include <cmath>

#include <QAbstractTableModel>
#include "../logic/manager.h"

class Manager;

class VisualModel : public QAbstractTableModel
{
public:
    explicit VisualModel() = default;
    QCustomPlot* plot_field = nullptr;
private:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
};
//    QModelIndex parent(const QModelIndex &child) const override { return {}; };
