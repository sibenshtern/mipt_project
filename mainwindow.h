#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QErrorMessage>
#include <QItemSelectionModel>

#include "models/datamodel.h"
#include "models/visualmodel.h"
#include "models/namingmodel.h"

#include "comboboxdelegate.h"
#include "colordelegate.h"

#include "plots/plotscatter.h"
#include "plots/plothist.h"
#include "plots/plotchoice.h"
#include "plots/plotscatter2d.h"

#include "logic/odf.h"

#include "parser/FormulaParser.hpp"
#include "logic/io.h"

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

    void loadFile();
    void saveFile(QString file_name);

    void on_AddVariableButton_clicked();

    void RemoveVariable();
    void RemoveMeasurement();

    void on_AddMeasurementButton_clicked();

    void plotOptions();
    void AddFormula();
    void AddTextBlock();
    void AddGraph();
    void AddTableBlock();
    void redraw();
private:
    Ui::MainWindow *ui;
    FormulaParser parser;

    DataModel *data_model{nullptr};
    VisualModel *visual_model{nullptr};
    NamingModel *naming_model{nullptr};
    InstrumentModel *instrument_model{nullptr};
    PlotChoice *plot{nullptr};
    ODF *odf_export{nullptr};

    IOCSV csv_io;
    IOJSON json_io;

    void keyPressEvent(QKeyEvent *event) override;

    QErrorMessage error_message;
};
#endif // MAINWINDOW_H
