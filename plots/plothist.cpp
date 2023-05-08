#include "plothist.h"

void PlotHist::draw(QCustomPlot* plot){
    auto m = Manager::instance();
    plot->clearGraphs();
    for (int i = 0; i < m->variables.size(); i++){
        auto & v = m->variables[i];
        if (!v.visual.visible) continue;
        plot->addGraph();
        int bins = 5;
        auto graph = plot->graph(plot->graphCount() - 1);
        QPen pen;
        pen.setColor(v.visual.color);
        pen.setStyle(v.visual.line_types[v.visual.line_type]);
        pen.setWidthF(v.visual.width);
        graph->setScatterStyle(v.visual.point_types["None"]);
        graph->setPen(pen);
        graph->setName(v.naming.full);
        graph->setLineStyle(QCPGraph::LineStyle::lsStepCenter);

        QCPErrorBars *errorBars = new QCPErrorBars(plot->xAxis, plot->yAxis);
        errorBars->removeFromLegend();
        errorBars->setDataPlottable(graph);
        double min = v.measurements[0];
        double max = v.measurements[0];
        for (auto e: v.measurements){
            min = std::min(min, e);
            max = std::max(max, e);
        }


        double step = (max - min) / bins;

        QVector<double> x, y;
        x.append(min - step/2);
        y.append(0);
        for (int j=0; j < bins; ++j)
        {
            double a = min + j * step;
            double b = min + (j + 1) * step;
            if (j == bins - 1) b += 1e-10;
            int count = 0;
            for (auto e: v.measurements) if (a <= e && e < b) count++;
            x.append((a + b)/2);
            y.append(count);
        }
        x.append(max+step/2);
        y.append(0);

        graph->setData(x, y);
        graph->rescaleAxes(true);
        plot->xAxis2->setVisible(true);
        plot->xAxis2->setTickLabels(false);
        plot->yAxis2->setVisible(true);
        plot->yAxis2->setTickLabels(false);
        plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    }
}

void PlotHist::options()
{
    PlotHistOptionsDialog optionDialog(xlabel, ylabel, title, this);
    optionDialog.show();
    optionDialog.exec();
    xlabel = optionDialog.xLabel.text();
    ylabel = optionDialog.yLabel.text();
    title = optionDialog.title.text();
    bins = (optionDialog.bins.text()).toInt();
}


PlotHistOptionsDialog::PlotHistOptionsDialog(QString xLabel, QString yLabel, QString title, QWidget* parent)
    :xLabel(xLabel), yLabel(yLabel), title(title), QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QLabel *titleLabel = new QLabel(tr("Plot title:"));
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(&this->title);

    QLabel *xLabelLabel = new QLabel(tr("X axis label:"));
    mainLayout->addWidget(xLabelLabel);
    mainLayout->addWidget(&this->xLabel);

    QLabel *yLabelLabel = new QLabel(tr("Y axis label:"));
    mainLayout->addWidget(yLabelLabel);
    mainLayout->addWidget(&this->yLabel);

    QLabel *binsField = new QLabel(tr("Bins"));
    mainLayout->addWidget(binsField);
    mainLayout->addWidget(&this->bins);
    setLayout(mainLayout);

}
