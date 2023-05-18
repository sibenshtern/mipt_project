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
    Manager::instance()->naming_model = naming_model;
    Manager::instance()->visual_model = visual_model;

    plot = new PlotChoice(QMap<QString, Plot *>{
            {"Scatter Plot",    new PlotScatter()},
            {"Histogramm Plot", new PlotHist()},
            {"2D Scatter Plot", new PlotScatter2D()}
    });
    Manager::instance()->plot = plot;
    plot->draw(ui->PlotWidget);

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
    connect(ui->AddFormulaButton, SIGNAL(clicked()), this, SLOT(AddFormula()));
    connect(ui->AddTextBlockButton, SIGNAL(clicked()), this, SLOT(AddTextBlock()));
    connect(ui->AddGraphButton, SIGNAL(clicked()), this, SLOT(AddGraph()));
    connect(ui->AddTableButton, SIGNAL(clicked()), this, SLOT(AddTableBlock()));
    connect(ui->RemoveVariableButton, SIGNAL(clicked()), this, SLOT(RemoveVariable()));
    connect(ui->RemoveMeasurementButton, SIGNAL(clicked()), this, SLOT(RemoveMeasurement()));
    connect(ui->LoadButton, SIGNAL(clicked()), this, SLOT(loadFile()));
    connect(ui->AddVariableButton, SIGNAL(clicked()), this, SLOT(addEmptyVariable()));
    connect(ui->ExportToODFButton, SIGNAL(clicked()), this, SLOT(ExportToODF()));
    connect(ui->errorCheckBox, SIGNAL(clicked()), this, SLOT(ShowError()));
}

void MainWindow::ExportToODF() {
    odf_export->ExportToODF();
}

void MainWindow::ShowError() {
    qDebug() << ui->errorCheckBox->isChecked();
    if (ui->errorCheckBox->isChecked())
        data_model->show_error = true;
    else
        data_model->show_error = false;
    ui->MainTable->repaint();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() == Qt::Key_S) {
            QString file_name = QFileDialog::getSaveFileName(this, tr("Save File"), "~", tr("CSV File (*.csv), JSON File(*.json)"));
            saveFile(file_name);
        }
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::DeleteODFBlock() {
    QVariant button = sender()->property("index");
    odf_export->DeleteBlock(odf_export->blocks[button.toInt()]);
    ui->area->repaint();
}

void MainWindow::MoveUpODFBlock() {
    QVariant button = sender()->property("index");
    odf_export->MoveUpBlock(odf_export->blocks[button.toInt()]);
    ui->area->repaint();
}

void MainWindow::MoveDownODFBlock() {
    QVariant button = sender()->property("index");
    odf_export->MoveDownBlock(odf_export->blocks[button.toInt()]);
    ui->area->repaint();
}

void MainWindow::saveFile(QString file_name) {
    QFile csv_file{file_name};

    if (!csv_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&csv_file);

    qDebug() << "MainWindow::save(file_name): " << file_name;

    if (file_name.endsWith(".csv"))
        out << csv_io.save();
    else if (file_name.endsWith(".json"))
        out << json_io.save().toJson(QJsonDocument::Indented); //TODO: Return it back!
}

void MainWindow::AddTextBlock() {
    odf_export->AddTextBlock();
    connect(odf_export->blocks.back()->delete_button, SIGNAL(clicked()), this, SLOT(DeleteODFBlock()));
    connect(odf_export->blocks.back()->up_button, SIGNAL(clicked()), this, SLOT(MoveUpODFBlock()));
    connect(odf_export->blocks.back()->down_button, SIGNAL(clicked()), this, SLOT(MoveDownODFBlock()));

}

void MainWindow::AddGraph() {
    odf_export->AddGraph(ui->PlotWidget);
    connect(odf_export->blocks.back()->delete_button, SIGNAL(clicked()), this, SLOT(DeleteODFBlock()));
    connect(odf_export->blocks.back()->up_button, SIGNAL(clicked()), this, SLOT(MoveUpODFBlock()));
    connect(odf_export->blocks.back()->down_button, SIGNAL(clicked()), this, SLOT(MoveDownODFBlock()));
}

void MainWindow::AddTableBlock() {
    odf_export->AddTableBlock();
    connect(odf_export->blocks.back()->delete_button, SIGNAL(clicked()), this, SLOT(DeleteODFBlock()));
    connect(odf_export->blocks.back()->up_button, SIGNAL(clicked()), this, SLOT(MoveUpODFBlock()));
    connect(odf_export->blocks.back()->down_button, SIGNAL(clicked()), this, SLOT(MoveDownODFBlock()));
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

void MainWindow::loadFile() {
    QString file_name;
    try {
        if (ui->ChooseSource->currentText() == "CSV file") {
            file_name = QFileDialog::getOpenFileName(this, tr("Open CSV file"), "~", tr("CSV File (*.csv)"));
            qDebug() << "MainWindow::loadFile(file_name): " << file_name;
            csv_io.load(file_name);
        } else if (ui->ChooseSource->currentText() == "JSON file") {
            file_name = QFileDialog::getOpenFileName(this, tr("Open JSON file"), "~", tr("JSON file (*.json)"));
            qDebug() << "MainWindow::loadFile(file_name): " << file_name;
            json_io.load(file_name);
        }
    } catch (std::runtime_error &e) {
        error_message.showMessage(e.what());
    }

    ui->MainTable->viewport()->repaint();
}

void MainWindow::addEmptyVariable() {
    Manager::instance()->AddVariable(VariableData{Manager::instance()->GetMeasurementsCount()});
}

void MainWindow::RemoveVariable() {
    auto selection_model = ui->MainTable->selectionModel();
    auto columns = selection_model->selectedColumns();

    if (columns.empty())
        return;

    size_t column_index = columns[0].column();

    for (size_t i = 0; i < columns.size(); ++i)
        Manager::instance()->DeleteVariable(column_index);

    selection_model->clearSelection();
    selection_model->clearCurrentIndex();
}

void MainWindow::RemoveMeasurement() {
    auto selection_model = ui->MainTable->selectionModel();
    auto rows = selection_model->selectedRows();

    if (rows.empty())
        return;

    size_t row_index = rows[0].row();

    for (size_t i = 0; i < rows.size(); ++i)
        Manager::instance()->DeleteMeasurement(row_index);

    selection_model->clearSelection();
    selection_model->clearCurrentIndex();
}

void MainWindow::on_AddMeasurementButton_clicked() {
    Manager::instance()->AddMeasurement();
    data_model->insertRow(Manager::instance()->GetMeasurementsCount());
    return;
}
