#include "instrumentmodel.h"

#include <iostream>
#include <QDebug>

int InstrumentModel::rowCount(const QModelIndex &parent) const {
    return Manager::instance()->GetVariablesCount();
}

int InstrumentModel::columnCount(const QModelIndex &parent) const {
    return 3;
}

bool InstrumentModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        int option = index.column();
        int variable = index.row();
        switch (option) {
            case 0:
                Manager::instance()->variables[variable].naming.full = value.toString();
                emit dataChanged(index, index);
                return true;
            case 1: {
                Manager::instance()->variables[variable].naming.alias = value.toString();
                emit dataChanged(index, index);
                return true;
            }
        }
    }
}

QVariant InstrumentModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            std::cout << "InstrumentModel::headerData(section): " << section << "\n";
            switch (section) {
                case 0:
                    return {"Instrument name"};
                case 1:
                    return {"Error Type"};
                case 2:
                    return {"Error Value"};
                default:
                    return {};
            }
        } else if (orientation == Qt::Vertical)
            return {Manager::instance()->variables[section].naming.alias};
    }
    return {};
}

Qt::ItemFlags InstrumentModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags default_flags{Qt::ItemIsSelectable, Qt::ItemIsEditable, Qt::ItemIsEnabled};
    return default_flags;
}

QVariant InstrumentModel::data(const QModelIndex &index, int role) const {
    auto manager = Manager::instance();
    if (index.row() < manager->GetVariablesCount()) {
        auto instrument = manager->instruments[index.row()];
        if (role == Qt::DisplayRole) {
            switch (index.column()) {
                case 0:
                    qInfo() << instrument.name;
                    return instrument.name;
                case 1: {
                    switch (instrument.type) {
                        case ErrorType::Absolute:
                            return {"Absolute"};
                        case ErrorType::Relative:
                            return {"Relative"};
                        case ErrorType::Calculated:
                            return {"Calculated"};
                    }
                }
                case 2:
                    switch (instrument.type) {
                        case ErrorType::Absolute:
                        case ErrorType::Relative:
                            return instrument.error.value;
                        case ErrorType::Calculated:
                            return {"Not supported"};
                    }
            }
        }
    }
    return {};
}

bool InstrumentModel::insertColumns(int column, int count, const QModelIndex &parent) {
    beginInsertColumns(QModelIndex{}, column, column + count - 1);
    endInsertColumns();
    return true;
}

bool InstrumentModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return true;
}

bool InstrumentModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);
    endRemoveRows();
    return true;
}

bool InstrumentModel::removeColumns(int column, int count, const QModelIndex &parent) {
    beginRemoveColumns(parent, column, column + count - 1);
    endRemoveColumns();
    return true;
}
