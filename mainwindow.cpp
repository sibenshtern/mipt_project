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

    Manager::instance()->data_model = data_model;
    Manager::instance()->instrument_model = instrument_model;

    Manager::instance()->AddVariable(VariableData{"x", "x", Instrument{}, QList<double>{1, 2, 2, 3, 4, 4, 4, 5, 6, 6, 7}});
    Manager::instance()->AddVariable(VariableData{"y", "y", Instrument{}, QList<double>{1, 3, 7, 4, 15, 5, 6, 6, 7}});
    Manager::instance()->AddInstrument(Instrument{ErrorType::Relative, 0.4, "Линейка"});
    Manager::instance()->AddInstrument(Instrument{ErrorType::Absolute, 10, "Стул"});
    Manager::instance()->plot = new PlotChoice(QMap<QString, Plot*> {
    {"Scatter Plot", new PlotScatter()}
    {"Histogramm Plot", new PlotHist()}
    });
    Manager::instance()->plot->draw(ui->PlotWidget);
    ui->VisualTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->MainTable->setModel(data_model);
    ui->VisualTable->setModel(visual_model);
    ui->NamingTable->setModel(naming_model);
    ui->InstrumentTable->setModel(instrument_model);
    visual_model->plot_field = ui->PlotWidget;
    QStringList PointTypes = (QStringList() << "None" << "Cross" << "Circle");
    QStringList LineTypes = (QStringList() << "Solid" << "Dashed" << "Dotted");
    ColorDelegate* PlotColorDelegate = new ColorDelegate(parent);
    ComboBoxDelegate* PointTypeDelegate = new ComboBoxDelegate(PointTypes, parent);
    ComboBoxDelegate* LineTypeDelegate = new ComboBoxDelegate(LineTypes, parent);
    ui->VisualTable->setItemDelegateForColumn(5, PointTypeDelegate);
    ui->VisualTable->setItemDelegateForColumn(2, LineTypeDelegate);
    ui->VisualTable->setItemDelegateForColumn(3, PlotColorDelegate);
    connect(ui->GraphSettingsButton, SIGNAL(clicked()), this, SLOT(plotOptions()));
    connect(ui->AddFormulaButton, SIGNAL(clicked()), this, SLOT(AddFormula()));
}

void MainWindow::AddFormula() {
    try {
        parser.parse(ui->lineEdit->text().toStdString());
        qInfo() << "finish parsing";
    }
    catch(std::exception &e) {
        error_message.showMessage(e.what());
    }

}

void MainWindow::plotOptions()
{
       Manager::instance()->plot->options();
}


void MainWindow::OpenGraphPage() {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::OpenDataPage() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::OpenReportPage(){
    ui->stackedWidget->setCurrentIndex(2);
}
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_LoadButton_clicked()
{
//    reader.load();
//    ui->MainTable->viewport()->repaint();
      return;
}



void MainWindow::on_AddVariableButton_clicked()
{
    Manager::instance()->variables.push_back(VariableData{Manager::instance()->GetMeasurementsCount()});
    data_model->insertColumn(Manager::instance()->GetVariablesCount() - 1);
    return;
}



void MainWindow::on_RemoveVariable_clicked()
{
    return;
}


void MainWindow::on_AddMeasurementButton_clicked()
{
    Manager::instance()->AddMeasurement();
    data_model->insertRow(Manager::instance()->GetMeasurementsCount());
    return;
}


void MainWindow::on_RemoveMeasurementButton_clicked()
{
    return;
}

