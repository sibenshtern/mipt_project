#include "visualmodel.h"
#include <iostream>
#include <QDebug>

int VisualModel::rowCount(const QModelIndex &parent) const {
    return Manager::instance()->GetVariablesCount();
}

int VisualModel::columnCount(const QModelIndex &parent) const {
    return 7;
}

enum VisualModelColumns{NameOfVariable, Visibility, LineType, Colour, LineWidth, PointType, ErrorBars};

Qt::ItemFlags VisualModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags default_flags {Qt::ItemIsSelectable, Qt::ItemIsEditable, Qt::ItemIsEnabled};
    switch (index.column())
    {
    case Visibility:
    case ErrorBars:
        return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    case NameOfVariable:
        return Qt::ItemIsEnabled;
    case LineType:
    case Colour:
    case LineWidth:
    case PointType:
        return default_flags;
}
}

QVariant VisualModel::data(const QModelIndex &index, int role) const {
   int variable = index.row();
    switch (index.column())
       {
        case NameOfVariable:
        if (role == Qt::DisplayRole)
            return QVariant(Manager::instance()->variables[variable].naming.full);
        else
            return QVariant();
        case Visibility:
        if (role == Qt::CheckStateRole)
           return QVariant(Manager::instance()->variables[variable].visual.visible);
        else
           return QVariant();
        case LineType:
        if (role == Qt::DisplayRole)
            return QVariant(Manager::instance()->variables[variable].visual.line_type);
        else
            return QVariant();
        case Colour:
        if (role == Qt::BackgroundRole)
            return QVariant(Manager::instance()->variables[variable].visual.color);
        else
              return QVariant();
        case LineWidth:
        if (role == Qt::DisplayRole)
           return QVariant(Manager::instance()->variables[variable].visual.width);
        else
            return QVariant();
        break;
        case PointType:
        if (role == Qt::DisplayRole)
           return QVariant(Manager::instance()->variables[variable].visual.point_type);
        else
           return QVariant();
        case ErrorBars:
        if (role == Qt::CheckStateRole)
            return QVariant(Manager::instance()->variables[variable].visual.error_bars);
        else
            return QVariant();


        }
}

QVariant VisualModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
                case NameOfVariable:
                    return "Name of variable";
                case Visibility:
                    return "Visibility";
                case LineType:
                    return "Line type";
                case Colour:
                    return "Colour";
                case LineWidth:
                    return "Line width";
                case PointType:
                    return "Point type";
                case ErrorBars:
                    return "Error Bars";
            }
        } else if (orientation == Qt::Vertical)
            return {section + 1};
    }
    return {};
}

bool VisualModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int option = index.column();
    int variable = index.row();
    switch(option)
    {
    case Visibility:
        Manager::instance()->variables[variable].visual.visible = !(Manager::instance()->variables[variable].visual.visible);
        Manager::instance()->plot->draw(Manager::instance()->plot_field);
        emit dataChanged(index, index);
        return true;
    case LineType:
    {
        Manager::instance()->variables[variable].visual.line_type = value.toString();
        Manager::instance()->plot->draw(Manager::instance()->plot_field);
        emit dataChanged(index, index);
        return true;
    }
    case Colour:
    {
        Manager::instance()->variables[variable].visual.color = value.value<QColor>();
        Manager::instance()->plot->draw(Manager::instance()->plot_field);
        emit dataChanged(index, index);
        return true;
    }
    case LineWidth:
    {
        Manager::instance()->variables[variable].visual.width = value.toInt();
        Manager::instance()->plot->draw(Manager::instance()->plot_field);
        emit dataChanged(index, index);
        return true;
    }
    case PointType:
    {
        Manager::instance()->variables[variable].visual.point_type = value.toString();
        Manager::instance()->plot->draw(Manager::instance()->plot_field);
        emit dataChanged(index, index);
        return true;
    case ErrorBars:
        Manager::instance()->variables[variable].visual.error_bars = !(Manager::instance()->variables[variable].visual.error_bars);
        Manager::instance()->plot->draw(Manager::instance()->plot_field);
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
