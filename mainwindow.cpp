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
    connect(ui->LoadButton, SIGNAL(clicked()), this, SLOT(load()));
    ui->menubar->addAction(data_action);
    ui->menubar->addAction(graph_action);
    ui->menubar->addAction(report_action);

    auto data_menu = new QMenu("Data", this);
    auto load_action = new QAction("Load", data_menu);
    data_menu->addAction(load_action);

    ui->menubar->addMenu(data_menu);

    data_model = new DataModel{};
    Manager::instance()->data_model = data_model;
    Manager::instance()->variables.push_back(VariableData{"123", "123", Instrument{}, QList<double>{1, 2, 3, 4}});

    ui->MainTable->setModel(data_model);
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

void MainWindow::save() {
    QFile file("data.csv");
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice:: Truncate);
    file.write(csv_reader.save().toUtf8());
}

void MainWindow::load()
{
    QString file_name = QFileDialog::getOpenFileName(nullptr,
                                                     "Open File", "$HOME", "CSV File (*csv)");
    csv_reader.load(file_name);
    ui->MainTable->viewport()->repaint();
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

