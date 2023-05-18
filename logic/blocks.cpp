#include "blocks.h"

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

TableBlock::TableBlock(QTableWidget *table) : table{table} {
    widget_layout->addWidget(table);
}

PlotBlock::PlotBlock(const QPixmap &plot_image, QLabel *image_label)
        : plot_image{plot_image}, image_label{image_label}
{
    image_label->setPixmap(plot_image);
    widget_layout->addWidget(image_label);
};