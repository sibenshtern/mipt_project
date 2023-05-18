#include "comboboxdelegate.h"

#include <QComboBox>

ComboBoxDelegate::ComboBoxDelegate(QStringList options, QObject* parent)
    : QStyledItemDelegate(parent), options(options) {}

ComboBoxDelegate::~ComboBoxDelegate() {}

QWidget* ComboBoxDelegate::createEditor(QWidget* parent,
                                        const QStyleOptionViewItem& option,
                                        const QModelIndex& index) const {
  return new QComboBox(parent);
}

void ComboBoxDelegate::setEditorData(QWidget* editor,
                                     const QModelIndex& index) const {
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  QString currentText = index.model()->data(index, Qt::DisplayRole).toString();
  for (auto& item : options) {
    combobox->addItem(item);
    if (currentText == item)
      combobox->setCurrentIndex(combobox->count() - 1);
  }
}

void ComboBoxDelegate::setModelData(QWidget* editor,
                                    QAbstractItemModel* model,
                                    const QModelIndex& index) const {
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  auto option = combobox->currentText();
  model->setData(index, option, Qt::EditRole);
}
