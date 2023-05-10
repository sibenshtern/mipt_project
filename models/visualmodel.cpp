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
    switch (index.column())
    {
    case 1:
        return default_flags | Qt::ItemIsUserCheckable;
    case 0:
        return Qt::ItemIsEnabled;
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
        if (role == Qt::DisplayRole)
            return QVariant(Manager::instance()->variables[variable].naming.full);
        else
            return QVariant();
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
        if (role == Qt::DisplayRole)
           return QVariant(Manager::instance()->variables[variable].visual.width);
        else
            return QVariant();
        break;
       case 5:
        if (role == Qt::DisplayRole)
           return QVariant(Manager::instance()->variables[variable].visual.point_type);
           else
               return QVariant();
        }
}


bool VisualModel::setData(const QModelIndex &index, const QVariant &value, int role)

{
    int option = index.column();
    int variable = index.row();
    switch(option)
    {
    case 1:
        Manager::instance()->variables[variable].visual.visible = !(Manager::instance()->variables[variable].visual.visible);
        Manager::instance()->plot->draw(plot_field);
        plot_field->replot();
        emit dataChanged(index, index);
        return true;
    case 2:
    {
        Manager::instance()->variables[variable].visual.line_type = value.toString();
        Manager::instance()->plot->draw(plot_field);
        plot_field->replot();
        emit dataChanged(index, index);
        return true;
    }
    case 3:
    {
        Manager::instance()->variables[variable].visual.color = value.value<QColor>();
        Manager::instance()->plot->draw(plot_field);
        plot_field->replot();
        emit dataChanged(index, index);
        return true;
    }
    case 4:
    {
        Manager::instance()->variables[variable].visual.width = value.toInt();
        Manager::instance()->plot->draw(plot_field);
        plot_field->replot();
        emit dataChanged(index, index);
        return true;
    }
    case 5:
    {
        Manager::instance()->variables[variable].visual.point_type = value.toString();
        Manager::instance()->plot->draw(plot_field);
        plot_field->replot();
        emit dataChanged(index, index);
        return true;
    }
    }
}
bool VisualModel::insertColumns(int column, int count, const QModelIndex &parent) {
    beginInsertColumns(QModelIndex{}, column, column + count - 1);
    endInsertColumns();
    return true;
}

bool VisualModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);
    std::cerr << Manager::instance()->GetMeasurementsCount();
    endInsertRows();
    return true;
}

bool VisualModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);
    endRemoveRows();
    return true;
}

bool VisualModel::removeColumns(int column, int count, const QModelIndex &parent) {
    beginRemoveColumns(parent, column, column + count - 1);
    endRemoveColumns();
    return true;
}
