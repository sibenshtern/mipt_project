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
    virtual void SaveToDocument() = 0;
    BlockType type = BlockType::None;
};

class TextBlock : public Block {
public:
    explicit TextBlock(QTextEdit *editor = new QTextEdit{}) : editor{editor} {}

    BlockType type = BlockType::TextBlock;
    void SaveToDocument() override {};

    QTextEdit *editor;
};

class TableBlock : public Block {
public:
    explicit TableBlock(QTableWidget *table = new QTableWidget{}) : table{table} {}

    BlockType type = BlockType::TableBlock;
    void SaveToDocument() override {};

    QTableWidget *table;
};

class PlotBlock : public Block {
public:
    explicit PlotBlock(const QPixmap &plot_image, QLabel *image_label = new QLabel{});

    BlockType type = BlockType::PlotBlock;
    void SaveToDocument() override {};

    QPixmap plot_image;
    QLabel *image_label;
};

#endif //DAFELAB_BLOCKS_H
