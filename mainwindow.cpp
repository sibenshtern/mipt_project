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
            {"Histogramm Plot", new PlotHist()}
    });
    Manager::instance()->plot = plot;
    plot->draw(ui->PlotWidget);

    ui->VisualTable->setEditTriggers(QAbstractItemView::AllEditTriggers);

    ui->MainTable->setModel(data_model);
    ui->VisualTable->setModel(visual_model);
    ui->NamingTable->setModel(naming_model);
    ui->InstrumentTable->setModel(instrument_model);
    Manager::instance()->plot_field = ui->PlotWidget;
    QStringList PointTypes = (QStringList() << "None" << "Cross" << "Circle");
    QStringList LineTypes = (QStringList() << "Solid" << "Dashed" << "Dotted");
    QStringList ErrorTypes = (QStringList() << "Relative" << "Absolute");

    auto *ErrorTypeDelegate = new ComboBoxDelegate(ErrorTypes, parent);

    auto *PlotColorDelegate = new ColorDelegate(parent);
    auto *PointTypeDelegate = new ComboBoxDelegate(PointTypes, parent);
    auto *LineTypeDelegate = new ComboBoxDelegate(LineTypes, parent);
    ui->VisualTable->setItemDelegateForColumn(2, LineTypeDelegate);
    ui->VisualTable->setItemDelegateForColumn(3, PlotColorDelegate);
    ui->VisualTable->setItemDelegateForColumn(5, PointTypeDelegate);

    ui->InstrumentTable->setItemDelegateForColumn(InstrumentModelColumns::ErrorType, ErrorTypeDelegate);

    connect(ui->GraphSettingsButton, SIGNAL(clicked()), this, SLOT(plotOptions()));
    connect(ui->AddFormulaButton, SIGNAL(clicked()), this, SLOT(AddFormula()));
    connect(ui->AddTextBlockButton, SIGNAL(clicked()), this, SLOT(AddTextBlock()));
    connect(ui->AddGraphButton, SIGNAL(clicked()), this, SLOT(AddGraph()));
    connect(ui->AddTableButton, SIGNAL(clicked()), this, SLOT(AddTableBlock()));
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
    return;
}

void MainWindow::on_RemoveVariable_clicked() {
    return;
}


void MainWindow::on_AddMeasurementButton_clicked() {
    Manager::instance()->AddMeasurement();
    data_model->insertRow(Manager::instance()->GetMeasurementsCount());
    return;
}


void MainWindow::on_RemoveMeasurementButton_clicked() {
    return;
}

