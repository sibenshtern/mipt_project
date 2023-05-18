#include "plotchoice.h"
#include "models/visualmodel.h"

VisualTableColumns vtc;

void PlotChoice::draw(QCustomPlot* plot)
{
    plots[currentPlot]->draw(plot);
}

void PlotChoice::options()
{
    PlotChoiceOptionsDialog optionDialog(currentPlot, plots.keys(), this);
    optionDialog.show();
    int result = optionDialog.exec();
    currentPlot = optionDialog.currentPlot.currentText();
    auto& table = Manager::instance()->visual_table;
    table->setModel(Manager::instance()->visual_model);
    if (currentPlot == "Histogramm Plot"){
        table->setColumnHidden(NameOfVariable, false);
        table->setColumnHidden(Visibility, false);
        table->setColumnHidden(LineType, false);
        table->setColumnHidden(Colour, false);
        table->setColumnHidden(LineWidth, false);
        table->setColumnHidden(ErrorBars, true);
        table->setColumnHidden(PointType, true);
        table->setRowHidden(1, false);
    }
    if (currentPlot == "Scatter Plot"){
        table->setColumnHidden(NameOfVariable, false);
        table->setColumnHidden(Visibility, false);
        table->setColumnHidden(LineType, false);
        table->setColumnHidden(Colour, false);
        table->setColumnHidden(LineWidth, false);
        table->setColumnHidden(ErrorBars, false);
        table->setColumnHidden(PointType, false);
        table->setRowHidden(1, false);
    }
    if (currentPlot == "2D Scatter Plot"){
        table->setColumnHidden(NameOfVariable, true);
        table->setColumnHidden(Visibility, true);
        table->setColumnHidden(LineType, true);
        table->setColumnHidden(Colour, false);
        table->setColumnHidden(LineWidth, true);
        table->setColumnHidden(ErrorBars, true);
        table->setColumnHidden(PointType, false);
        table->setRowHidden(1, true);

    }
    if (result == QDialog::Rejected || result == QDialog::Accepted) // TODO: Fix dialog opening after clicking on screen
        Manager::instance()->plot->draw(Manager::instance()->plot_field);
}

void PlotChoice::optionsCurrentPlot(QString currentPlot)
{
    plots[currentPlot]->options();
}

PlotChoiceOptionsDialog::PlotChoiceOptionsDialog(QString currentPlot, QStringList allPlots, QWidget* parent)
    :QDialog(parent)
    {
        QVBoxLayout *mainLayout = new QVBoxLayout;
        QLabel *currentPlotLabel = new QLabel(tr("Chosen plot type:"));
        mainLayout->addWidget(currentPlotLabel);
        this->currentPlot.insertItems(0, allPlots);
        this->currentPlot.setCurrentText(currentPlot);
        mainLayout->addWidget(&this->currentPlot);
        QLabel* currentPlotOptionsLabel = new QLabel(tr("Modify current plot options:"));
        mainLayout->addWidget(currentPlotOptionsLabel);
        QPushButton *currentPlotOptions = new QPushButton("Options");
        mainLayout->addWidget(currentPlotOptions);
        connect(currentPlotOptions, SIGNAL(clicked()), this, SLOT(options()));
        setLayout(mainLayout);
    }

void PlotChoiceOptionsDialog::options()
{
    static_cast<PlotChoice*>(parent())->optionsCurrentPlot(currentPlot.currentText());
}
