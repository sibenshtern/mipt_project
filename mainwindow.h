#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "logic/readers.h"

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
    StrategyIOCSV reader;
    DataModel *data_model{nullptr};
};
#endif // MAINWINDOW_H
