#ifndef PLOTCHOICE_H
#define PLOTCHOICE_H
#include <QDialog>
#include "../logic/variable_data.h"
#include "plot.h"
#include "../logic/manager.h"

class PlotChoiceOptionsDialog: public QDialog
{
    Q_OBJECT
public:
    explicit PlotChoiceOptionsDialog(QString currentPlot, QStringList allPlots, QWidget*parent = nullptr);
    QComboBox currentPlot;
private slots:
    void options();
//    void optionsCurrentPlot(QString currentPlot);
};

class PlotChoice: public Plot
{
    Q_OBJECT
public:
    PlotChoice(QMap<QString, Plot*> plots, QWidget* parent = nullptr):
        plots(plots), currentPlot("Scatter Plot"), Plot(parent){};
    ~PlotChoice() = default;

    void draw(QCustomPlot* plot) override;
    void options() override;
private:
    QMap<QString, Plot*> plots;
    QString currentPlot;
public slots:
    void optionsCurrentPlot(QString currentPlot);
};

#endif // PLOTCHOICE_H
