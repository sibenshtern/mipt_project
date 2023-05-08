#ifndef PLOTHIST_H
#define PLOTHIST_H
#include <QWidget>
#include "../logic/variable_data.h"
#include "plot.h"
#include "../logic/manager.h"
#include <QDialog>

class PlotHistOptionsDialog : public QDialog
{
public:
    explicit PlotHistOptionsDialog(QString, QString, QString, QWidget*);
    QLineEdit xLabel, yLabel, title, bins;
};



class PlotHist : public Plot
{
    Q_OBJECT
public:
    PlotHist(QWidget* parent = nullptr) : Plot(parent) {};
    ~PlotHist() = default;

    void draw(QCustomPlot* plot) override;
    void options() override;
    int bins = 5;
    QString xlabel = "Measurement number", ylabel = "Measurement value", title = "";
};

#endif // PLOTHIST_H
