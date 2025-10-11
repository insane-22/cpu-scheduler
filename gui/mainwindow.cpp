#include "mainwindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "implementation.hpp"
#include "simulator.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    auto *controls = new QHBoxLayout();
    controls->setSpacing(10);
    controls->addWidget(new QLabel("Algorithm:"));
    algoCombo_ = new QComboBox();
    algoCombo_->addItems({"FCFS", "RoundRobin", "SJF", "SRTF"});
    controls->addWidget(algoCombo_);
    controls->addWidget(new QLabel("Quantum:"));
    quantumSpin_ = new QSpinBox();
    quantumSpin_->setMinimum(1);
    quantumSpin_->setValue(2);
    controls->addWidget(quantumSpin_);
    runBtn_ = new QPushButton("Run");
    runBtn_->setFixedWidth(80);
    controls->addWidget(runBtn_);

    controls->addStretch(1);
    mainLayout->addLayout(controls);

    auto *procLayout = new QHBoxLayout();
    procTable_ = new QTableWidget(0, 3);
    procTable_->setHorizontalHeaderLabels({"PID", "Arrival Time", "Burst Time"});
    procTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    procTable_->setEditTriggers(QAbstractItemView::AllEditTriggers);
    procTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    procTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    procTable_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    procLayout->addWidget(procTable_);

    auto *procBtns = new QVBoxLayout();
    addRowBtn_ = new QPushButton("Add Process");
    delRowBtn_ = new QPushButton("Delete Selected");
    procBtns->addWidget(addRowBtn_);
    procBtns->addWidget(delRowBtn_);
    procBtns->addStretch(1);
    procLayout->addLayout(procBtns);
    mainLayout->addLayout(procLayout);

    auto *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);
    gantt_ = new GanttWidget();
    gantt_->setMinimumHeight(250);
    gantt_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    contentLayout->addWidget(gantt_, 3);

    statsTable_ = new QTableWidget(7, 2);
    statsTable_->setHorizontalHeaderLabels({"Metric", "Value"});
    statsTable_->verticalHeader()->setVisible(false);
    statsTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    statsTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    statsTable_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    statsTable_->setFixedWidth(250);

    const QStringList metrics = {
        "Total Time taken",
        "Avg Waiting Time",
        "Avg Turnaround Time",
        "Avg Response Time",
        "CPU Util (%)",
        "Throughput",
        "Context Switches"
    };
    for (int i = 0; i < metrics.size(); i++)
        statsTable_->setItem(i, 0, new QTableWidgetItem(metrics[i]));

    contentLayout->addWidget(statsTable_, 1);
    mainLayout->addLayout(contentLayout, 1);

    setCentralWidget(central);

    connect(runBtn_, &QPushButton::clicked, this, &MainWindow::onRunClicked);
    connect(algoCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onAlgoChanged);
    connect(addRowBtn_, &QPushButton::clicked, this, &MainWindow::onAddProcess);
    connect(delRowBtn_, &QPushButton::clicked, this, &MainWindow::onDeleteProcess);

    onAlgoChanged(algoCombo_->currentIndex());
}

MainWindow::~MainWindow() {}

void MainWindow::onAlgoChanged(int idx) {
    quantumSpin_->setEnabled(idx == 1);
}

void MainWindow::onAddProcess() {
    int row = procTable_->rowCount();
    procTable_->insertRow(row);
    procTable_->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
    procTable_->setItem(row, 1, new QTableWidgetItem("0"));
    procTable_->setItem(row, 2, new QTableWidgetItem("1"));
}

void MainWindow::onDeleteProcess() {
    int row = procTable_->currentRow();
    if (row >= 0)
        procTable_->removeRow(row);
}

void MainWindow::onRunClicked() {
    std::vector<Task> tasks;

    for (int i = 0; i < procTable_->rowCount(); i++) {
        bool ok1, ok2, ok3;
        int pid = procTable_->item(i, 0)->text().toInt(&ok1);
        int arrival = procTable_->item(i, 1)->text().toInt(&ok2);
        int burst = procTable_->item(i, 2)->text().toInt(&ok3);
        if (ok1 && ok2 && ok3)
            tasks.emplace_back(pid, arrival, burst);
    }

    if (tasks.empty()) return;

    Result res;
    if (algoCombo_->currentText() == "FCFS") {
        FCFS sched;
        res = simulate(tasks, sched, 200);
    }else if(algoCombo_->currentText() == "SJF") {
        SJF sched;
        std::unordered_map<int,int> bm;
        for (auto &t : tasks) bm[t.pid] = t.burst;
        sched.set_bursts(bm);
        res = simulate(tasks, sched, 200);
    } else if(algoCombo_->currentText() == "SRTF") {
        SRTF sched;
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

    setVal(0, QString::number(res.total_ticks));
    setVal(1, QString::number(res.avg_waiting, 'f', 2));
    setVal(2, QString::number(res.avg_turnaround, 'f', 2));
    setVal(3, QString::number(res.avg_response, 'f', 2));
    setVal(4, QString::number(res.cpu_utilization, 'f', 2));
    setVal(5, QString::number(res.throughput, 'f', 4));
    setVal(6, QString::number(res.context_switches));
}
