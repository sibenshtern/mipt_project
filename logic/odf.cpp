#include "odf.h"
#include "manager.h"

ODF::ODF(QWidget *widget_with_blocks) : widget_with_blocks{widget_with_blocks} {
    document = new QTextDocument{};
    cursor = new QTextCursor{};
}

void ODF::ExportToODF() {
    document = new QTextDocument();
    cursor = new QTextCursor(document);

    for (auto block : blocks) {
        block->Export(cursor);
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "untitled",tr("Open Document (.odf)"));
    QTextDocumentWriter odfWritter(fileName); odfWritter.write(document);
}

void ODF::DeleteBlock(Block *block) {
    blocks.erase(blocks.begin() + blocks.indexOf(block));
    widget_with_blocks->layout()->removeWidget(block->widget);
    block->widget->hide();

    for (size_t i = 0; i < blocks.size(); ++i)
        blocks[static_cast<int>(i)]->SetProperty(i);
}

void ODF::MoveUpBlock(Block *block) {
    auto tmp_block = blocks[blocks.indexOf(block)];
    int index = blocks.indexOf(block);
    if (index > 0) {
        blocks.erase(blocks.begin() + blocks.indexOf(block));
        blocks.insert(index - 1, tmp_block);
        widget_with_blocks->layout()->removeWidget(tmp_block->widget);
        dynamic_cast<QVBoxLayout *>(widget_with_blocks->layout())->insertWidget(index - 1, tmp_block->widget);

        for (size_t i = 0; i < blocks.size(); ++i)
            blocks[static_cast<int>(i)]->SetProperty(i);
    }
}

void ODF::MoveDownBlock(Block *block) {
    auto tmp_block = blocks[blocks.indexOf(block)];
    int index = blocks.indexOf(block);

    if (index < static_cast<int>(blocks.size()) - 1) {
        blocks.erase(blocks.begin() + blocks.indexOf(block));
        blocks.insert(index + 1, tmp_block);
        widget_with_blocks->layout()->removeWidget(tmp_block->widget);
        dynamic_cast<QVBoxLayout *>(widget_with_blocks->layout())->insertWidget(index + 1, tmp_block->widget);

        for (size_t i = 0; i < blocks.size(); ++i)
            blocks[static_cast<int>(i)]->SetProperty(i);
    }
}

void ODF::AppendBlock(Block *block) {
    blocks.append(block);

    auto layout = qobject_cast<QVBoxLayout *>(widget_with_blocks->layout());
    if (layout && block->widget) {
        layout->addWidget(block->widget);
        block->SetProperty(blocks.size() - 1);
    }
    widget_with_blocks->update();
}

void ODF::AddTextBlock() {
    AppendBlock(new TextBlock());
}

void ODF::AddGraph(QCustomPlot *plot) {
    AppendBlock(new PlotBlock(plot->toPixmap(400, 300)));
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

//    auto *item = new QTableWidgetItem(Qt::)
    AppendBlock(new TableBlock(table));
}