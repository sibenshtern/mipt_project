#include "odf.h"

ODF::ODF(QWidget *widget_with_blocks) : widget_with_blocks{widget_with_blocks} {
    document = new QTextDocument{};
    cursor = new QTextCursor{};
}

void ODF::AddTextBlock() {
    blocks.append(new TextBlock());

    auto layout = qobject_cast<QVBoxLayout *>(widget_with_blocks->layout());
    if (layout)
        layout->addWidget(dynamic_cast<TextBlock *>(blocks.back())->editor);
    else
        qDebug() << "Cannot find layout";
    widget_with_blocks->update();
}

void ODF::AddGraph(QCustomPlot *plot) {
    blocks.append(new PlotBlock(plot->toPixmap(400, 300)));

    auto layout = qobject_cast<QVBoxLayout *>(widget_with_blocks->layout());
    if (layout)
        layout->addWidget(dynamic_cast<PlotBlock *>(blocks.back())->image_label);
    else
        qDebug() << "Cannot find layout";
    widget_with_blocks->update();
}

void ODF::AddTableBlock() {}