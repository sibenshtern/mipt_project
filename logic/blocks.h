#ifndef DAFELAB_BLOCKS_H
#define DAFELAB_BLOCKS_H

#include <QTextEdit>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

enum class BlockType {
    TextBlock, TableBlock, PlotBlock, None
};

class Block {
public:
    virtual void Export(QTextCursor *document) = 0;
    Block();
    BlockType type = BlockType::None;

    void DeleteBlock(QWidget *widget);
    void SetProperty(size_t index);

    QWidget *widget;
    QPushButton *up_button;
    QPushButton *down_button;
    QPushButton *delete_button;
    QVBoxLayout *button_layout;
    QHBoxLayout *widget_layout;
};

class TextBlock : public Block {
public:
    explicit TextBlock(QTextEdit *editor = new QTextEdit{});

    void Export(QTextCursor *cursor) override;

    QTextEdit *editor;
};

class TableBlock : public Block {
public:
    explicit TableBlock(QTableWidget *table = new QTableWidget{});

    void Export(QTextCursor *cursor) override;

    QTableWidget *table;
};

class PlotBlock : public Block {
public:
    explicit PlotBlock(const QPixmap &plot_image, QLabel *image_label = new QLabel{});

    void Export(QTextCursor *cursor) override;

    QPixmap plot_image;
    QImage image;
    QLabel *image_label;
};

#endif //DAFELAB_BLOCKS_H
