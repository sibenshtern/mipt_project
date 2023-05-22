#include "namingmodel.h"
#include <iostream>
#include <QDebug>

int NamingModel::rowCount(const QModelIndex &parent) const {
    return Manager::instance()->GetVariablesCount();
}

int NamingModel::columnCount(const QModelIndex &parent) const {
    return 2;
}


bool NamingModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        int option = index.column();
        int variable = index.row();
        switch(option)
        {
        case 0:
            if (value.toString() == "")
                return false;
            Manager::instance()->variables[variable].naming.full = value.toString();
            emit dataChanged(index, index);
            return true;
        case 1:
        {
            if (value.toString() == "")
                return false;
           Manager::instance()->variables[variable].naming.alias = value.toString();
           emit dataChanged(index, index);
           return true;
        }
        }
    }
}

QVariant NamingModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch(section) {
                case 0:
                    return "Full name";
                case 1:
                    return "Short name";
            }
        } else if (orientation == Qt::Vertical)
            return {section + 1};
    }
    return {};
}

Qt::ItemFlags NamingModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags default_flags {Qt::ItemIsSelectable, Qt::ItemIsEditable, Qt::ItemIsEnabled};
         return default_flags;
}

QVariant NamingModel::data(const QModelIndex &index, int role) const {
   int variable = index.row();
    switch (index.column())
       {
        case 0:
        if (role == Qt::DisplayRole)
            return QVariant(Manager::instance()->variables[variable].naming.full);
        else
            return QVariant();
       case 1:
        if (role == Qt::DisplayRole)
           return QVariant(Manager::instance()->variables[variable].naming.alias);
        else
            return QVariant();
        }
}

bool NamingModel::insertColumns(int column, int count, const QModelIndex &parent) {
    beginInsertColumns(QModelIndex{}, column, column + count - 1);
    endInsertColumns();
    return true;
}

bool NamingModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);
    std::cerr << Manager::instance()->GetMeasurementsCount();
    endInsertRows();
    return true;
}

bool NamingModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);
    endRemoveRows();
    return true;
}

bool NamingModel::removeColumns(int column, int count, const QModelIndex &parent) {
    beginRemoveColumns(parent, column, column + count - 1);
    endRemoveColumns();
    return true;
}
