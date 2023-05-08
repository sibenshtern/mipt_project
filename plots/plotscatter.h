#ifndef PLOTSCATTER_H
#define PLOTSCATTER_H
#include <QWidget>
#include <logic/variable_data.h>
#include "plot.h"
#include "logic/manager.h"
#include <QDialog>

class PlotScatterOptionsDialog : public QDialog
{
public:
    explicit PlotScatterOptionsDialog(QString, QString, QString, QWidget*);
    QLineEdit xLabel, yLabel, title;
};

class PlotScatter: public Plot
{
    Q_OBJECT
public:
    PlotScatter(QWidget* parent = nullptr): Plot(parent){};
    ~PlotScatter() = default;

    void draw(QCustomPlot* plot) override;
    void options() override;
    QString xlabel = "Measurement number", ylabel = "Measurement value", title = "";
};





#endif // PLOTSCATTER_H
