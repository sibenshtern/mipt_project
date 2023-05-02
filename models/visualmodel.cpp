#include "visualmodel.h"
#include <iostream>
#include <QDebug>


int VisualModel::rowCount(const QModelIndex &parent) const {
    return Manager::instance()->GetVariablesCount();
}

int VisualModel::columnCount(const QModelIndex &parent) const {
    return 6;
}

Qt::ItemFlags VisualModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags default_flags {Qt::ItemIsSelectable, Qt::ItemIsEditable, Qt::ItemIsEnabled};

    switch (index.column()) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            return default_flags;
    }
}

QVariant VisualModel::data(const QModelIndex &index, int role) const {
   int variable = index.row();
    switch (index.column())
       {
        case 0:
            return QVariant("Name of variable");
       case 1:
        if (role == Qt::CheckStateRole)
           return QVariant(Manager::instance()->variables[variable].visual.visible);
        else
            return QVariant();
       case 2:
        if (role == Qt::DisplayRole)
            return QVariant(Manager::instance()->variables[variable].visual.line_type);
            else
            return QVariant();
       case 3:
          if (role == Qt::BackgroundRole)
            return QVariant(Manager::instance()->variables[variable].visual.color);
          else
              return QVariant();
       case 4:
           return QVariant(Manager::instance()->variables[variable].visual.width);
       case 5:
           return QVariant(Manager::instance()->variables[variable].visual.point_type);
        }
}

// TODO: remove std::cout
//QVariant VisualModel::data(const QModelIndex &index, int role) const {
//    return {};
//}

bool VisualModel::setData(const QModelIndex &index, const QVariant &value, int role)

{
    int option = index.column();
    int variable = index.row();
    switch(option)
    {
    if (role == Qt::EditRole)
    case 1:
        Manager::instance()->variables[0].visual.visible = !(Manager::instance()->variables[variable].visual.visible);
        emit dataChanged(index, index);
        return true;
        break;
    case 2:
        Manager::instance()->variables[variable].visual.line_type = value.toString();
        emit dataChanged(index, index);
        return true;
        break;
    case 3:
        Manager::instance()->variables[variable].visual.color = value.toString();
        emit dataChanged(index, index);
        return true;
        break;

    case 4:
        Manager::instance()->variables[variable].visual.width = value.toInt();
        emit dataChanged(index, index);
        return true;
        break;
    case 5:
        Manager::instance()->variables[variable].visual.point_type = value.toString();
        emit dataChanged(index, index);
        return true;
        break;
    }

}

//Qt::ItemFlags DataModel::flags(const QModelIndex &index) const {
//    return {Qt::ItemIsSelectable, Qt::ItemIsEditable, Qt::ItemIsEnabled};
//}

//bool DataModel::insertColumns(int column, int count, const QModelIndex &parent) {
//    beginInsertColumns(QModelIndex{}, column, column);
//    endInsertColumns();
//    return true;
//}

//bool DataModel::insertRows(int row, int count, const QModelIndex &parent) {
//    beginInsertRows(parent, row, row + count - 1);
//    std::cerr << Manager::instance()->GetMeasurementsCount();
//    endInsertRows();
//    return true;
