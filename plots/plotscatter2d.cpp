#include "plotscatter2d.h"

void PlotScatter2D::draw(QCustomPlot* plot)
{
    plot->clearGraphs();
    plot->addGraph();
    auto graph = plot->graph(0);
    QPen pen;
    for (int i = 0; i < Manager::instance()->variables.size();++i)
        if (Manager::instance()->variables[i].naming.full == x_variable.naming.full)
            break;
    for (int i = 0; i < Manager::instance()->variables.size();++i){
        bool flag = false;
        if (Manager::instance()->variables[i].naming.full == y_variable.naming.full){
            break;
            flag = true;
        }
        if (flag) return;
    }
    pen.setColor("green");
    pen.setStyle(Qt::SolidLine);
    pen.setWidthF(5);
    graph->setScatterStyle(QCPScatterStyle::ssNone);
    graph->setPen(pen);
    graph->setLineStyle(QCPGraph::LineStyle::lsNone);

    QCPErrorBars *errorBarsY = new QCPErrorBars(plot->xAxis, plot->yAxis);
    QCPErrorBars *errorBarsX = new QCPErrorBars(plot->yAxis, plot->xAxis);
    errorBarsX->removeFromLegend();
    errorBarsY->removeFromLegend();
    errorBarsX->setDataPlottable(graph);
    errorBarsY->setDataPlottable(graph);
    QVector<double> x, y, eX, eY;
    for (int j=0; j<std::min(x_variable.measurements.size(), y_variable.measurements.size()); ++j)
    {
      x.append(x_variable.measurements[j]);
      y.append(y_variable.measurements[j]);
      eX.append(x_variable.Error(j));
      eY.append(y_variable.Error(j));
    }
    graph->setData(x, y);
    errorBarsX->setData(eX);
    errorBarsY->setData(eY);
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
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    plot->replot();
}
void PlotScatter2D::options()
{
    PlotScatter2DOptionsDialog optionDialog(xlabel, ylabel, title, this);
    optionDialog.show();
    optionDialog.exec();
    xlabel = Manager::instance()->GetVariable(optionDialog.xLabel.currentText()).naming.full;
    x_variable =  Manager::instance()->GetVariable(optionDialog.xLabel.currentText());
    y_variable =  Manager::instance()->GetVariable(optionDialog.yLabel.currentText());
    ylabel = Manager::instance()->GetVariable(optionDialog.yLabel.currentText()).naming.full;
    title = optionDialog.title.text();
}

PlotScatter2DOptionsDialog::PlotScatter2DOptionsDialog(QString xlabel, QString ylabel, QString title, QWidget* parent)
    : title(title), QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QLabel* titleLabel = new QLabel(tr("Plot title:"));
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(&this->title);

    QLabel *xLabelLabel = new QLabel(tr("X axis label"));
    mainLayout->addWidget(xLabelLabel);
    this->xLabel.clear();
    for (int i = 0; i < Manager::instance()->variables.size();++i){
        this->xLabel.addItem(Manager::instance()->variables[i].naming.full);
        if(Manager::instance()->variables[i].naming.full == xlabel)
            this->xLabel.setCurrentIndex(i);
    }
    mainLayout->addWidget(&this->xLabel);

    QLabel *yLabelLabel = new QLabel(tr("Y axis label"));
    mainLayout->addWidget(yLabelLabel);
    this->yLabel.clear();
    for (int i = 0; i < Manager::instance()->variables.size();++i){
        this->yLabel.addItem(Manager::instance()->variables[i].naming.full);
        if(Manager::instance()->variables[i].naming.full == ylabel)
            this->yLabel.setCurrentIndex(i);
    }
    mainLayout->addWidget(&this->yLabel);
    setLayout(mainLayout);
  }
