#include <QApplication>
#include "mainwindow.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("CPU Scheduler Visualizer (Qt)");
    w.show();
    return a.exec();
}
