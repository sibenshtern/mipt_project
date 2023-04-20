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
        try {
            answer = QString::number(Manager::instance()->variables[index.column()][index.row()]);
        } catch (std::range_error &error) {
            answer = "";
        }
//        std::cout << answer.toStdString() << " ";
        return {answer};
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
    beginInsertColumns(QModelIndex{}, column, column);
    endInsertColumns();
    return true;
}

bool DataModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);
    std::cerr << Manager::instance()->GetMeasurementsCount();
    endInsertRows();
    return true;
}
