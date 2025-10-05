#include "mainwindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "implementation.hpp"
#include "simulator.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);

    auto *controls = new QHBoxLayout();
    controls->addWidget(new QLabel("Algorithm:"));
    algoCombo_ = new QComboBox();
    algoCombo_->addItem("FCFS");
    algoCombo_->addItem("RoundRobin");
    controls->addWidget(algoCombo_);
    controls->addWidget(new QLabel("Quantum:"));
    quantumSpin_ = new QSpinBox();
    quantumSpin_->setMinimum(1);
    quantumSpin_->setValue(2);
    controls->addWidget(quantumSpin_);
    runBtn_ = new QPushButton("Run");
    controls->addWidget(runBtn_);

    mainLayout->addLayout(controls);

    gantt_ = new GanttWidget();
    mainLayout->addWidget(gantt_, 1);

    statsTable_ = new QTableWidget(6, 2);
    statsTable_->setHorizontalHeaderLabels(QStringList() << "Metric" << "Value");
    statsTable_->verticalHeader()->setVisible(false);
    statsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    statsTable_->setItem(0,0, new QTableWidgetItem("Avg Waiting"));
    statsTable_->setItem(1,0, new QTableWidgetItem("Avg Turnaround"));
    statsTable_->setItem(2,0, new QTableWidgetItem("Avg Response"));
    statsTable_->setItem(3,0, new QTableWidgetItem("CPU Util (%)"));
    statsTable_->setItem(4,0, new QTableWidgetItem("Throughput"));
    statsTable_->setItem(5,0, new QTableWidgetItem("Context Switches"));
    mainLayout->addWidget(statsTable_);

    setCentralWidget(central);

    connect(runBtn_, &QPushButton::clicked, this, &MainWindow::onRunClicked);
    connect(algoCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onAlgoChanged);

    onAlgoChanged(algoCombo_->currentIndex());
}

MainWindow::~MainWindow() {}

void MainWindow::onAlgoChanged(int idx) {
    quantumSpin_->setEnabled(idx == 1);
}

void MainWindow::onRunClicked() {
    std::vector<Task> tasks = {
        Task(1, 0, 5),
        Task(2, 2, 3),
        Task(3, 4, 8),
        Task(4, 6, 2)
    };

    Result res;
    if (algoCombo_->currentText() == "FCFS") {
        FCFS sched;
        res = simulate(tasks, sched, 200);
    } else {
        int q = quantumSpin_->value();
        RoundRobin sched(q);
        res = simulate(tasks, sched, 200);
    }

    gantt_->setResult(res);

    auto setVal = [&](int row, const QString &v){
        statsTable_->setItem(row, 1, new QTableWidgetItem(v));
    };

    setVal(0, QString::number(res.avg_waiting, 'f', 2));
    setVal(1, QString::number(res.avg_turnaround, 'f', 2));
    setVal(2, QString::number(res.avg_response, 'f', 2));
    setVal(3, QString::number(res.cpu_utilization, 'f', 2));
    setVal(4, QString::number(res.throughput, 'f', 4));
    setVal(5, QString::number(res.context_switches));
}
