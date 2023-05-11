#include "blocks.h"

PlotBlock::PlotBlock(const QPixmap &plot_image, QLabel *image_label)
        : plot_image{plot_image}, image_label{image_label}
{
    image_label->setPixmap(plot_image);
};