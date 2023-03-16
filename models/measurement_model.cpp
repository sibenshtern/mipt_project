#include <iostream>

#include "measurement_model.h"

MeasurementModel::MeasurementModel(Manager *manager) : QAbstractItemModel{} {
    p_manager = manager;
};

int MeasurementModel::rowCount(const QModelIndex &parent) const {
    return p_manager->GetMeasurementsCount();
}

int MeasurementModel::columnCount(const QModelIndex &parent) const {
    return p_manager->GetVariablesCount();
}

QVariant MeasurementModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        QString answer;
        try {
            answer = QString::number(p_manager->GetVariable(index.column()).GetMeasurement(index.row()));
            std::cout << answer.toStdString() << "\n";
        } catch (std::invalid_argument &error) {
            answer = "";
        }
        return {answer};
    }
    return {};
}

QModelIndex MeasurementModel::index(int row, int column, const QModelIndex &parent) const {
    return createIndex(row, column);
}

bool MeasurementModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        if (p_manager->GetVariable(index.column()).GetMeasurement(index.row()) != value.toDouble()) {
            p_manager->GetVariable(index.column()).ChangeMeasurement(index.row(), value.toDouble());
            emit dataChanged(index, index, {Qt::EditRole, Qt::DisplayRole});
            return true;
        }
        return false;
    }
}

Qt::ItemFlags MeasurementModel::flags(const QModelIndex &index) const {
    return Qt::ItemFlags{Qt::ItemIsSelectable, Qt::ItemIsEditable, Qt::ItemIsEnabled};
}

