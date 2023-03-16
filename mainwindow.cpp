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
    return;
}



void MainWindow::on_AddVariableButton_clicked()
{
    return;
}



void MainWindow::on_RemoveVariable_clicked()
{
    return;
}


void MainWindow::on_AddMeasurementButton_clicked()
{
    return;
}


void MainWindow::on_RemoveMeasurementButton_clicked()
{
    return;
}

