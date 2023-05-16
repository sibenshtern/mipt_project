#ifndef PLOTSCATTER2D_H
#define PLOTSCATTER2D_H
#include <QWidget>
#include "../logic/variable_data.h"
#include "plots/plot.h"
#include "../logic/manager.h"
#include <QDialog>


class PlotScatter2DOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotScatter2DOptionsDialog(QString xLabel, QString yLabel, QString title, QWidget *parent);
    QComboBox xLabel, yLabel;
    QLineEdit title;
};

class PlotScatter2D: public Plot
{

public:
//    Manager(QString xLabel, QString yLabel, QWidget* parent = nullptr): xLabel{xlabel}, yLabel{ylabel}, Plot(parent) {};
    PlotScatter2D() : Plot(){};
    ~PlotScatter2D() = default;
    void draw(QCustomPlot* plot) override;
    void options() override;
    VariableData x_variable, y_variable;
    QString xlabel = "Measurement number", ylabel = "Measurement value", title = "";
};

#endif // PLOTSCATTER2D_H
