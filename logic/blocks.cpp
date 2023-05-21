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
    QTextTableFormat table_format;
    table_format.setCellPadding(5);
    table_format.setAlignment(Qt::AlignCenter);

    cursor->insertTable(table->rowCount(), table->columnCount(), table_format);

    for (int row = 0; row < table->rowCount(); ++row) {
        for (int column = 0; column < table->columnCount(); ++column) {
            cursor->insertText(table->item(row, column)->text());
            cursor->movePosition(QTextCursor::NextCell);
        }
    }
    cursor->movePosition(QTextCursor::NextBlock);
}

PlotBlock::PlotBlock(const QPixmap &plot_image, QLabel *image_label)
        : plot_image{plot_image}, image_label{image_label}
{
    image_label->setPixmap(plot_image);
    widget_layout->addWidget(image_label);
    image = plot_image.toImage();
};

void PlotBlock::Export(QTextCursor *cursor) {
    static unsigned long image_n = 0;
    QString url = QString("mydata://image" + QString::number(image_n) + ".png");

    cursor->document()->addResource(QTextDocument::ImageResource, QUrl(url), plot_image.toImage());

    QTextImageFormat image_format;
    image_format.setName(url);
    image_format.setQuality(100);

    cursor->insertImage(image_format);
    cursor->insertBlock();
    image_n++;
}