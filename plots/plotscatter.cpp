#include "plotscatter.h"

void PlotScatter::draw(QCustomPlot *plot)
{
    auto m = Manager::instance();
    plot->clearGraphs();
    for (int i = 0; i < m->variables.size(); i++){
        auto & v = m->variables[i];
        if (!v.visual.visible) continue;
        plot->addGraph();
        auto graph = plot->graph(plot->graphCount() - 1);
        QPen pen;
        pen.setColor(v.visual.color);
        pen.setStyle(v.visual.line_types[v.visual.line_type]);
        pen.setWidthF(v.visual.width);
        graph->setScatterStyle(v.visual.point_types[v.visual.point_type]);
        graph->setPen(pen);
        graph->setName(v.naming.full);

        QCPErrorBars *errorBars = new QCPErrorBars(plot->xAxis, plot->yAxis);
        errorBars->removeFromLegend();
        errorBars->setDataPlottable(graph);
        QVector<double> x, y, e;
        for (int j=0; j<m->variables[i].measurements.size(); ++j)
        {
          x.append(j);
          y.append(v.measurements[j]);
          e.append(1); //TODO:: add errors
        }
        graph->setData(x, y);
        errorBars->setData(e);
    }
        if (plot->plotLayout()->children().size() <= 1)
        {
            plot->plotLayout()->insertRow(0);
            plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, title));
        }
        static_cast<QCPTextElement*>(plot->plotLayout()->element(0, 0))->setText(title);
        plot->xAxis->setLabel(xlabel);
        plot->yAxis->setLabel(ylabel);
        plot->xAxis->setVisible(true);
        plot->yAxis->setVisible(true);
        plot->legend->setVisible(true);
        plot->legend->setBrush(QColor(255, 255, 255, 150));
        plot->rescaleAxes();
        plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        plot->replot();
}
void PlotScatter::options()
{
    PlotScatterOptionsDialog optionDialog(xlabel, ylabel, title, this);
    optionDialog.show();
    optionDialog.exec();
    xlabel = optionDialog.xLabel.text();
    ylabel = optionDialog.yLabel.text();
    title = optionDialog.title.text();
}

PlotScatterOptionsDialog::PlotScatterOptionsDialog(QString xlabel, QString ylabel, QString title, QWidget* parent)
     : xLabel(xlabel), yLabel(ylabel), title(title), QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QLabel* titleLabel = new QLabel(tr("Plot title:"));
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(&this->title);

    QLabel* xLabelLabel = new QLabel(tr("X axis label:"));
    mainLayout->addWidget(xLabelLabel);
    mainLayout->addWidget(&this->xLabel);

    QLabel* yLabelLabel = new QLabel(tr("Y axis label:"));
    mainLayout->addWidget(yLabelLabel);
    mainLayout->addWidget(&this->yLabel);

    setLayout(mainLayout);


}


