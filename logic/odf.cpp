#include "odf.h"
#include "manager.h"

ODF::ODF(QWidget *widget_with_blocks) : widget_with_blocks{widget_with_blocks} {
    document = new QTextDocument{};
    cursor = new QTextCursor{};
}

void ODF::AddTextBlock() {
    blocks.append(new TextBlock());

    auto layout = qobject_cast<QVBoxLayout *>(widget_with_blocks->layout());
    if (layout) {
        auto text_block = dynamic_cast<TextBlock *>(blocks.back());
        if (text_block)
            layout->addWidget(text_block->editor);
        else
            qWarning() << "Cannot convert block to TextBlock";
    } else
        qWarning() << "Cannot find layout";
    widget_with_blocks->update();
}

void ODF::AddGraph(QCustomPlot *plot) {
    blocks.append(new PlotBlock(plot->toPixmap(400, 300)));

    auto layout = qobject_cast<QVBoxLayout *>(widget_with_blocks->layout());
    if (layout) {
        auto plot_block = dynamic_cast<PlotBlock *>(blocks.back());
        if (plot_block)
            layout->addWidget(plot_block->image_label);
        else
            qWarning() << "Cannot convert block to PlotBlock";
    }
    else
        qWarning() << "Cannot find layout";
    widget_with_blocks->update();
}

void ODF::AddTableBlock() {
    auto manager = Manager::instance();
    int rows_count = manager->GetMeasurementsCount();
    int columns_count = manager->GetVariablesCount();
    auto table = new QTableWidget(rows_count, columns_count);

    for (int row = 0; row < rows_count; ++row) {
        for (int column = 0; column < columns_count; ++column) {
            QString number = QString::number(manager->variables[column].measurements[row]);
            QVariant variant(number);

            auto *item = new QTableWidgetItem();
            item->setData(Qt::DisplayRole, variant);
            table->setItem(row, column, item);
        }
    }
    blocks.append(new TableBlock(table));

    auto layout = qobject_cast<QVBoxLayout *>(widget_with_blocks->layout());
    if (layout) {
        auto table_block = dynamic_cast<TableBlock *>(blocks.back());
        if (table_block)
            layout->addWidget(table_block->table);
        else
            qWarning() << "Cannot convert block to PlotBlock";
    }
    else
        qWarning() << "Cannot find layout";
    widget_with_blocks->update();
}