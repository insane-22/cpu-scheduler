#include "ganttwidget.hpp"
#include <QPainter>
#include <QColor>
#include <algorithm>

GanttWidget::GanttWidget(QWidget *parent) : QWidget(parent) {
    setMinimumHeight(300);
    setMinimumWidth(600);
}

void GanttWidget::setResult(const Result &r) {
    res_ = r;
    update(); 
}

void GanttWidget::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.fillRect(rect(), Qt::white);
    const int margin_left = 80;
    const int margin_top = 20;
    const int lane_height = 40;
    const int lane_gap = 12;

    std::vector<int> pids;
    pids.reserve(res_.run_intervals.size());
    for (auto &kv : res_.run_intervals) pids.push_back(kv.first);
    std::sort(pids.begin(), pids.end());

    int lanes = std::max(1, (int)pids.size());
    int content_width = width() - margin_left - 20;
    int total_ticks = std::max(1, res_.total_ticks);
    double scale = (double)content_width / (double)total_ticks;

    p.setPen(QPen(Qt::lightGray));
    for (int t = 0; t <= total_ticks; ++t) {
        int x = margin_left + (int)(t * scale);
        p.drawLine(x, margin_top, x, margin_top + lanes * (lane_height + lane_gap));
        if (t % std::max(1, total_ticks/10) == 0) {
            p.drawText(x+2, margin_top - 2, QString::number(t));
        }
    }

    int i = 0;
    for (int pid : pids) {
        int y = margin_top + i * (lane_height + lane_gap);
        p.setPen(Qt::black);
        p.drawText(5, y + lane_height/2 + 5, QString("P%1").arg(pid));

        auto &segments = res_.run_intervals[pid];
        QColor color = QColor::fromHsv((pid * 45) % 360, 200, 200);
        for (auto &seg : segments) {
            int sx = margin_left + (int)(seg.first * scale);
            int sw = std::max(2, (int)((seg.second - seg.first) * scale));
            QRect rseg(sx, y, sw, lane_height);
            p.fillRect(rseg, color);
            p.setPen(Qt::black);
            p.drawRect(rseg);
            p.drawText(rseg.left() + 4, rseg.top() + lane_height/2 + 5, QString("P%1").arg(pid));
        }
        i++;
    }
}
