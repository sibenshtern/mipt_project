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
//    auto* ps = new PlotScatter;
//    ps->draw(ui->PlotWidget);

    data_model = new DataModel{};
    visual_model = new VisualModel{};
    Manager::instance()->data_model = data_model;
//    Manager::instance()->graphs = ui->PlotWidget; //
    Manager::instance()->variables.push_back(VariableData{"123", "123", Instrument{}, QList<double>{1, 2, 3, 4}});

    ui->MainTable->setModel(data_model);
    ui->VisualTable->setModel(visual_model);
    QStandardItem* item0 = new QStandardItem(true);
    item0->setCheckable(true);
    item0->setCheckState(Qt::Checked);
    item0->setText("some text");
    QStringList PointTypes = (QStringList() << "None" << "C" << "Dotted");
    QStringList LineTypes = (QStringList() << "Solid" << "Dashed" << "Dotted");
    ColorDelegate* PlotColorDelegate = new ColorDelegate(parent);
    ComboBoxDelegate* PointTypeDelegate = new ComboBoxDelegate(PointTypes, parent);
    ComboBoxDelegate* LineTypeDelegate = new ComboBoxDelegate(LineTypes, parent);
    ui->VisualTable->setItemDelegateForColumn(5, PointTypeDelegate);
    ui->VisualTable->setItemDelegateForColumn(2, LineTypeDelegate);
    ui->VisualTable->setItemDelegateForColumn(3, PlotColorDelegate);



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

