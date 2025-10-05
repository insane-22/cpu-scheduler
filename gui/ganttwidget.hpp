#ifndef GANTTWIDGET_HPP
#define GANTTWIDGET_HPP

#include <QWidget>
#include "scheduler.hpp"

class GanttWidget : public QWidget {
    Q_OBJECT
public:
    explicit GanttWidget(QWidget *parent = nullptr);
    void setResult(const Result &r);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    Result res_;
};

#endif
