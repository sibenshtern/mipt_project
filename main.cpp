#include "mainwindow.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow b;
    MainWindow w;
    w.show();
    return a.exec();
}
