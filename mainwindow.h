#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//#include "logic/readers.h"
#include "qcustomplot.h"
#include "models/datamodel.h"
#include "models/visualmodel.h"
#include "comboboxdelegate.h"
#include "colordelegate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void OpenGraphPage();
    void OpenDataPage();
    void OpenReportPage();

    void on_LoadButton_clicked();

    void on_AddVariableButton_clicked();

    void on_RemoveVariable_clicked();

    void on_AddMeasurementButton_clicked();

    void on_RemoveMeasurementButton_clicked();

private:
    Ui::MainWindow *ui;
//    StrategyIOCSV reader;
    DataModel *data_model{nullptr};
    VisualModel *visual_model{nullptr};
};
#endif // MAINWINDOW_H
