#include "visualmodel.h"
#include <iostream>
#include <QDebug>

int VisualModel::rowCount(const QModelIndex &parent) const {
    return Manager::instance()->GetMeasurementsCount();
}

int VisualModel::columnCount(const QModelIndex &parent) const {
    return 5;
}

Qt::ItemFlags VisualModel::flags(const QModelIndex &index) const {
    return {Qt::ItemIsSelectable, Qt::ItemIsEditable, Qt::ItemIsEnabled};
}


QVariant VisualModel::data(const QModelIndex &index, int role) const {
        return QVariant();

}

// TODO: remove std::cout
//QVariant VisualModel::data(const QModelIndex &index, int role) const {
//    return {};
//}


//bool VisualModel::setData(const QModelIndex &index, const QVariant &value, int role) {
//    int variable = index.row();
//    int option = index.column();
//    auto& visual = Manager::instance()->variables[variable].visual;
//    if (role == Qt::CheckStateRole){
//        case 0:
//            if (!value.canConvert<int>()) return false;
//            if (value.toInt() < Qt::Unchecked || value.toInt() > Qt::Checked) return false; //check later!
//            auto state = static_cast<Qt::CheckState>(value.toInt());
//            visual.visible = (state == Qt::Checked);
//    }


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
//}
