#include "plotchoice.h"

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
