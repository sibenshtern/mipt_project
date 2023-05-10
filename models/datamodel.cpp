#include <iostream>
#include <QDebug>

#include "datamodel.h"

int DataModel::rowCount(const QModelIndex &parent) const {
    return Manager::instance()->GetMeasurementsCount();
}

int DataModel::columnCount(const QModelIndex &parent) const {
    return Manager::instance()->GetVariablesCount();
}

// TODO: remove std::cout
QVariant DataModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        QString answer;
        std::cout << "DataModel::data(row, column): " << index.row() << " " << index.column() << "\n";
        if (index.column() < Manager::instance()->variables.size())
            try {
                answer = QString::number(Manager::instance()->variables[index.column()][index.row()]);
            } catch (std::range_error &error) {
                answer = "";
            }
        return {answer};
    }
    return {};
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole)
        if (orientation == Qt::Horizontal) {
            std::cout << "DataModel::headerData(section): " << section << "\n";
            if (section < Manager::instance()->variables.size())
                return Manager::instance()->variables[section].naming.alias;
            else
                return {""};
        } else if (orientation == Qt::Vertical) {
            return {section + 1};
        }
    return {};
}

QModelIndex DataModel::index(int row, int column, const QModelIndex &parent) const {
    return createIndex(row, column);
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        if (Manager::instance()->variables[index.column()][index.row()] != value.toDouble()) {
            try {
                Manager::instance()->variables[index.column()][index.row()] = value.toDouble();
                return true;
            } catch (...) {
                std::cout << "ooops" << "\n";
            }
        }
        return false;
    }
    return true;
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const {
    return {Qt::ItemIsSelectable, Qt::ItemIsEditable, Qt::ItemIsEnabled};
}

bool DataModel::insertColumns(int column, int count, const QModelIndex &parent) {
    beginInsertColumns(QModelIndex{}, column, column + count - 1);
    endInsertColumns();
    return true;
}

bool DataModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);
    std::cerr << Manager::instance()->GetMeasurementsCount();
    endInsertRows();
    return true;
}

bool DataModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);
    endRemoveRows();
    return true;
}

bool DataModel::removeColumns(int column, int count, const QModelIndex &parent) {
    beginRemoveColumns(parent, column, column + count - 1);
    endRemoveColumns();
    return true;

}
