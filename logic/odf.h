#ifndef DAFELAB_ODF_H
#define DAFELAB_ODF_H

#include <QHBoxLayout>
#include <QTextDocument>
#include <QTextBlock>
#include <QWidget>
#include <QMap>
#include "../qcustomplot.h"

#include "blocks.h"

class ODF : public QWidget {
    Q_OBJECT;
public:
    explicit ODF(QWidget *widget_with_blocks);
    ~ODF() override { delete document; delete cursor; }

    void AddTextBlock();
    void AddGraph(QCustomPlot *plot);
    void AddTableBlock();
    void DeleteBlock(Block *block);
    void MoveUpBlock(Block *block);
    void MoveDownBlock(Block *block);
    void AppendBlock(Block *block);
    void ExportToODF();
    QList<Block *> blocks;
private:
    QWidget *widget_with_blocks;
    QTextDocument *document;
    QTextCursor *cursor;
};

#endif //DAFELAB_ODF_H

