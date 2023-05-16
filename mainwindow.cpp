#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    auto *graph_action = new QAction("Graph", this);
    auto *data_action = new QAction("Data", this);
    auto *report_action = new QAction("Report", this);

    connect(data_action, SIGNAL(triggered()), this, SLOT(OpenDataPage()));
    connect(graph_action, SIGNAL(triggered()), this, SLOT(OpenGraphPage()));
    connect(report_action, SIGNAL(triggered()), this, SLOT(OpenReportPage()));

    ui->menubar->addAction(data_action);
    ui->menubar->addAction(graph_action);
    ui->menubar->addAction(report_action);
    data_model = new DataModel{};
    visual_model = new VisualModel{};
    naming_model = new NamingModel{};
    instrument_model = new InstrumentModel{};
    odf_export = new ODF{ui->area};

    Manager::instance()->data_model = data_model;
    Manager::instance()->instrument_model = instrument_model;

    Manager::instance()->AddVariable(
            VariableData{"x", "x", Instrument{}, QList<double>{1, 2, 2, 3, 4, 4, 4, 5, 6, 6, 7}});
    Manager::instance()->AddVariable(VariableData{"y", "y", Instrument{}, QList<double>{1, 3, 7, 4, 15, 5, 6, 6, 7}});
    Manager::instance()->AddInstrument(Instrument{ErrorType::Relative, 0.4, "Линейка"});
    Manager::instance()->AddInstrument(Instrument{ErrorType::Absolute, 10, "Стул"});

    plot = new PlotChoice(QMap<QString, Plot *>{
            {"Scatter Plot",    new PlotScatter()},
            {"Histogramm Plot", new PlotHist()},
            {"2D Scatter Plot", new PlotScatter2D()}
    });
    Manager::instance()->plot = plot;
    plot->draw(ui->PlotWidget);

    ui->VisualTable->setEditTriggers(QAbstractItemView::AllEditTriggers);

    ui->MainTable->setModel(data_model);
    ui->VisualTable->setModel(visual_model);
    ui->NamingTable->setModel(naming_model);
    ui->InstrumentTable->setModel(instrument_model);
    Manager::instance()->plot_field = ui->PlotWidget;
    QStringList PointTypes = (QStringList() << "None" << "Cross" << "Circle"); // TODO: Fix names
    QStringList LineTypes = (QStringList() << "Solid" << "Dashed" << "Dotted"); // TODO: Fix names
    QStringList ErrorTypes = (QStringList() << "Relative" << "Absolute");

    auto *ErrorTypeDelegate = new ComboBoxDelegate(ErrorTypes, parent);

    auto *PlotColorDelegate = new ColorDelegate(parent);
    auto *PointTypeDelegate = new ComboBoxDelegate(PointTypes, parent);
    auto *LineTypeDelegate = new ComboBoxDelegate(LineTypes, parent);
    Manager::instance()->visual_table = ui->VisualTable;
    Manager::instance()->scatters_models = visual_model;
    Manager::instance()->td_scatter_model = instrument_model;
    ui->VisualTable->setItemDelegateForColumn(2, LineTypeDelegate);
    ui->VisualTable->setItemDelegateForColumn(3, PlotColorDelegate);
    ui->VisualTable->setItemDelegateForColumn(5, PointTypeDelegate);

    ui->InstrumentTable->setItemDelegateForColumn(InstrumentModelColumns::ErrorType, ErrorTypeDelegate);

    connect(ui->GraphSettingsButton, SIGNAL(clicked()), this, SLOT(plotOptions()));
    connect(ui->RedrawButton, SIGNAL(clicked()), this, SLOT(redraw()));
    connect(ui->AddFormulaButton, SIGNAL(clicked()), this, SLOT(AddFormula()));
    connect(ui->AddTextBlockButton, SIGNAL(clicked()), this, SLOT(AddTextBlock()));
    connect(ui->AddGraphButton, SIGNAL(clicked()), this, SLOT(AddGraph()));
    connect(ui->AddTableButton, SIGNAL(clicked()), this, SLOT(AddTableBlock()));
    connect(ui->RemoveVariableButton, SIGNAL(clicked()), this, SLOT(RemoveVariable()));
    connect(ui->RemoveMeasurementButton, SIGNAL(clicked()), this, SLOT(RemoveMeasurement()));
    connect(ui->RedrawButton, SIGNAL(QDialog::Rejected), this, SLOT(redraw()));
}

void MainWindow::AddTextBlock() {
    odf_export->AddTextBlock();
}

void MainWindow::AddGraph() {
    odf_export->AddGraph(ui->PlotWidget);
}

void MainWindow::AddTableBlock() {
    odf_export->AddTableBlock();
}

void MainWindow::AddFormula() {
    try {
        parser.parse(ui->lineEdit->text().toStdString());
        qInfo() << "finish parsing";
    }
    catch (std::exception &e) {
        error_message.showMessage(e.what());
    }
}

void MainWindow::redraw() {
    Manager::instance()->plot->draw(ui->PlotWidget);
}

void MainWindow::plotOptions() {
    Manager::instance()->plot->options();
}

void MainWindow::OpenGraphPage() {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::OpenDataPage() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::OpenReportPage() {
    ui->stackedWidget->setCurrentIndex(2);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_LoadButton_clicked() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), "~", tr("CSV File (*.csv)"));
    csv_reader.load(filename);
    ui->MainTable->viewport()->repaint();
}


void MainWindow::on_AddVariableButton_clicked() {
    Manager::instance()->variables.push_back(VariableData{Manager::instance()->GetMeasurementsCount()});
    data_model->insertColumn(Manager::instance()->GetVariablesCount() - 1);
}

void MainWindow::RemoveVariable() {
    auto selection_model = ui->MainTable->selectionModel();
    size_t column_index = selection_model->currentIndex().column();

    qInfo() << "MainWindow::RemoveVariable(column_index): " << column_index;

    auto manager = Manager::instance();

    if (column_index < manager->variables.size())
        manager->variables.erase(manager->variables.begin() + static_cast<int>(column_index));
    else if (column_index - manager->variables.size() < manager->calculated.size())
        manager->calculated.erase(manager->calculated.begin() + static_cast<int>(column_index - manager->variables.size()));

    data_model->removeColumn(static_cast<int>(column_index));
    selection_model->clearSelection();
    selection_model->clearCurrentIndex();
}

void MainWindow::RemoveMeasurement() {
    auto selection_model = ui->MainTable->selectionModel();
    size_t row_index = selection_model->currentIndex().row();

    qInfo() << selection_model->currentIndex();
    qInfo() << "MainWindow::RemoveVariable(column_index): " << row_index;

    auto manager = Manager::instance();

    if (row_index < manager->GetMeasurementsCount()) {
        for (auto &_variable : manager->variables)
            _variable.measurements.erase(_variable.measurements.begin() + static_cast<int>(row_index));
        for (auto &calculate : manager->calculated)
            calculate.measurements.erase(calculate.measurements.begin() + static_cast<int>(row_index));
    }

    data_model->removeRow(static_cast<int>(row_index));
    selection_model->clearSelection();
    selection_model->clearCurrentIndex();

}

void MainWindow::on_AddMeasurementButton_clicked() {
    Manager::instance()->AddMeasurement();
    data_model->insertRow(Manager::instance()->GetMeasurementsCount());
    return;
}
