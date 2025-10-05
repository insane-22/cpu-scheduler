#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include "ganttwidget.hpp"
#include <QHeaderView>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onRunClicked();
    void onAlgoChanged(int idx);
private:
    QComboBox *algoCombo_;
    QSpinBox *quantumSpin_;
    QPushButton *runBtn_;
    GanttWidget *gantt_;
    QTableWidget *statsTable_;
};

#endif 
