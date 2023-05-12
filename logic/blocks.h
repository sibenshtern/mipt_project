#ifndef DAFELAB_BLOCKS_H
#define DAFELAB_BLOCKS_H

#include <QTextEdit>
#include <QTableWidget>
#include <QLabel>

enum class BlockType {
    TextBlock, TableBlock, PlotBlock, None
};

class Block {
public:
    virtual void Export() = 0;
    BlockType type = BlockType::None;
};

class TextBlock : public Block {
public:
    explicit TextBlock(QTextEdit *editor = new QTextEdit{}) : editor{editor} {}

    void Export() override {};

    QTextEdit *editor;
};

class TableBlock : public Block {
public:
    explicit TableBlock(QTableWidget *table = new QTableWidget{}) : table{table} {}

    void Export() override {};

    QTableWidget *table;
};

class PlotBlock : public Block {
public:
    explicit PlotBlock(const QPixmap &plot_image, QLabel *image_label = new QLabel{});

    void Export() override {};

    QPixmap plot_image;
    QLabel *image_label;
};

#endif //DAFELAB_BLOCKS_H
