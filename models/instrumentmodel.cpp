#include "instrumentmodel.h"

#include <QDebug>

int InstrumentModel::rowCount(const QModelIndex &parent) const {
    return Manager::instance()->GetVariablesCount();
}

int InstrumentModel::columnCount(const QModelIndex &parent) const {
    return 3;
}

bool InstrumentModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole && value.toString() != "") {
        switch (index.column()) {
            case InstrumentModelColumns::Name:
                Manager::instance()->variables[index.row()].instrument.name = value.toString();
            case InstrumentModelColumns::ErrorType: {
                QString error_type = value.toString();
                if (error_type == "Relative")
                    Manager::instance()->variables[index.row()].instrument.error.type = ErrorType::Relative;
                else if (error_type == "Absolute")
                    Manager::instance()->variables[index.row()].instrument.error.type = ErrorType::Absolute;
                return true;
            }
            case InstrumentModelColumns::ErrorValue: {
                if (value.toString().contains(','))
                    const_cast<QVariant &>(value) = QVariant{value.toString().replace(',', '.')};
                Manager::instance()->variables[index.row()].instrument.error.value = value.toDouble();
                Manager::instance()->plot->draw(Manager::instance()->plot_field);
                emit dataChanged(index, index);
                return true;
            }
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

QVariant InstrumentModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
                case InstrumentModelColumns::Name:
                    return {"Instrument name"};
                case InstrumentModelColumns::ErrorType:
                    return {"Error Type"};
                case InstrumentModelColumns::ErrorValue:
                    return {"Error Value"};
                default:
                    return {};
            }
        } else if (orientation == Qt::Vertical) {
            return {Manager::instance()->variables[section].naming.alias};
        }
    }
    return {};
}

Qt::ItemFlags InstrumentModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags default_flags{Qt::ItemIsSelectable, Qt::ItemIsEditable, Qt::ItemIsEnabled};
    if (Manager::instance()->variables[index.row()].instrument.error.type == ErrorType::Calculated && index.column() >= 1) {
        return {Qt::ItemIsSelectable, Qt::ItemIsEnabled};
    }
    return default_flags;
}

QVariant InstrumentModel::data(const QModelIndex &index, int role) const {
    auto manager = Manager::instance();

    if (index.isValid() && index.row() < manager->GetVariablesCount()) {
        auto instrument = manager->variables[index.row()].instrument;
        if (role == Qt::DisplayRole) {
            switch (index.column()) {
                case InstrumentModelColumns::Name:
                    return instrument.name;
                case InstrumentModelColumns::ErrorType: {
                    switch (instrument.error.type) {
                        case ErrorType::Absolute:
                            return {"Absolute"};
                        case ErrorType::Relative:
                            return {"Relative"};
                        case ErrorType::Calculated:
                            return {"Calculated"};
                    }
                }
                case InstrumentModelColumns::ErrorValue:
                    if (instrument.error.type != ErrorType::Calculated)
                        return instrument.error.value;
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
