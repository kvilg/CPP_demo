#include "RealTimePlot.h"

#include <QtCharts/QChart>

#include <QPen>
#include <QtGlobal>

RealTimePlot::RealTimePlot(QWidget *parent) : QChartView(parent) {
    m_series = new QLineSeries();

    auto *chart = new QChart();
    chart->addSeries(m_series);
    chart->legend()->hide();
    chart->createDefaultAxes();

    m_axisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first());
    m_axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());

    m_axisX->setRange(0, m_window);
    m_axisY->setRange(-10, 10);

    setChart(chart);
    setRenderHint(QPainter::Antialiasing);
}

void RealTimePlot::addPoint(double y) {
    m_series->append(m_t, y);
    m_t += 1;

    if (m_t > m_window)
        m_axisX->setRange(m_t - m_window, m_t);

    const auto pts = m_series->points();
    const int start = qMax(0, pts.size() - m_window);
    double minY =  1e18, maxY = -1e18;
    for (int i = start; i < pts.size(); ++i) {
        minY = qMin(minY, pts[i].y());
        maxY = qMax(maxY, pts[i].y());
    }
    if (minY <= maxY) {
        double margin = qMax(1.0, (maxY - minY) * 0.1);
        m_axisY->setRange(minY - margin, maxY + margin);
    }

    if (m_series->count() > 2000)
        m_series->removePoints(0, m_series->count() - 2000);
}

void RealTimePlot::reset() {
    m_series->clear();
    m_t = 0;
    m_axisX->setRange(0, m_window);
    m_axisY->setRange(-10, 10);
}

void RealTimePlot::setCurveColor(const QColor &color) {
    QPen pen = m_series->pen();
    pen.setColor(color);
    m_series->setPen(pen);
    m_series->setColor(color);
}