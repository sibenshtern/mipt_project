#include <QTextTable>
#include "blocks.h"
#include "manager.h"

void Block::DeleteBlock(QWidget *blocks_widget) {
    blocks_widget->layout()->removeWidget(widget);
}

Block::Block() {
    widget = new QWidget();

    up_button = new QPushButton{"↑", widget};
    delete_button = new QPushButton("×", widget);
    down_button = new QPushButton("↓", widget);

    up_button->setMaximumSize(20, 20);
    delete_button->setMaximumSize(20, 20);
    down_button->setMaximumSize(20, 20);

    button_layout = new QVBoxLayout{};
    widget_layout = new QHBoxLayout{widget};

    button_layout->addWidget(up_button);
    button_layout->addWidget(delete_button);
    button_layout->addWidget(down_button);
    button_layout->setAlignment(Qt::AlignmentFlag::AlignCenter);
    widget_layout->setAlignment(Qt::AlignmentFlag::AlignLeft);

    widget_layout->addLayout(button_layout);
}

void Block::SetProperty(size_t index) {
    up_button->setProperty("index", QVariant{static_cast<int>(index)});
    delete_button->setProperty("index", QVariant{static_cast<int>(index)});
    down_button->setProperty("index", QVariant(static_cast<int>(index)));
}

TextBlock::TextBlock(QTextEdit *editor) : editor{editor} {
    widget_layout->addWidget(editor);
}

void TextBlock::Export(QTextCursor *cursor) {
    cursor->insertText(editor->toPlainText());
    cursor->insertBlock();
}

TableBlock::TableBlock(QTableWidget *table) : table{table} {
    widget_layout->addWidget(table);
}

void TableBlock::Export(QTextCursor *cursor) {
    QTextTable *document_table = cursor->insertTable(table->rowCount() + 1, table->columnCount() + 1);

    for (int row = 1; row <= table->rowCount(); ++row) {
        auto cell = document_table->cellAt(row, 0);
        auto cellCursor = cell.firstCursorPosition();
        cellCursor.insertText(QString::number(row));
    }

    for (int column = 1; column <= table->columnCount(); ++column) {
        auto cell = document_table->cellAt(0, column);
        auto cellCursor = cell.firstCursorPosition();
        cellCursor.insertText(Manager::instance()->variables[column - 1].naming.full);
    }

    for (int row = 1; row <= table->rowCount(); ++row) {
        for (int column = 1; column <= table->columnCount(); ++column) {
            auto cell = document_table->cellAt(row, column);
            auto cellCursor = cell.firstCursorPosition();
            qDebug() << "TableBlock::Export" << table->itemAt(row - 1, column - 1)->text();
            cellCursor.insertText(table->item(row - 1, column - 1)->text());
        }
    }
    cursor->insertBlock();
}

PlotBlock::PlotBlock(const QPixmap &plot_image, QLabel *image_label)
        : plot_image{plot_image}, image_label{image_label}
{
    image_label->setPixmap(plot_image);
    widget_layout->addWidget(image_label);
};

void PlotBlock::Export(QTextCursor *cursor) {
    static size_t image_n = 0;
    QString url = QString("mydata://image%1.png").arg(image_n);

    QImage image = plot_image.toImage();
    cursor->document()->addResource(QTextDocument::ImageResource, QUrl(url), image);

    QTextImageFormat image_format;
    image_format.setQuality(100);
    image_format.setName("table" + QString::number(image_n));

    cursor->insertImage(image_format);
    cursor->insertBlock();

    image_n++;
}