#include "RealTimePlot.h"

#include <QtCharts/QChart>

#include <QPen>
#include <QtGlobal>

RealTimePlot::RealTimePlot(QWidget* parent) : QChartView(parent) {
    m_series = new QLineSeries();

    auto* chart = new QChart();
    chart->addSeries(m_series);
    chart->legend()->hide();
    chart->createDefaultAxes();

    // Две непроверенные операции в одной строке: first() на пустом
    // списке — UB, а qobject_cast может вернуть nullptr, который тут же
    // разыменовывается ниже. Сегодня работает, потому что тип осей
    // зависит от типа серии, а не от её наполнения, и для QLineSeries
    // createDefaultAxes делает QValueAxis. Поменяют тип серии или
    // переставят вызовы — получат падение в конструкторе.
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

    if (m_t > m_window) {
        m_axisX->setRange(m_t - m_window, m_t);
    }

    const auto pts = m_series->points();
    const int start = qMax(0, pts.size() - m_window);
    // 1e18 взяты с потолка и ничего не говорят. Проще начать с первой
    // точки окна, pts[start]: она там есть всегда, выше append уже
    // добавил.
    //
    // Тогда уходят не только магические числа, но и проверка
    // minY <= maxY ниже: она нужна ровно для случая, когда цикл не
    // выполнился ни разу и значения остались начальными.
    double minY = 1e18, maxY = -1e18;
    for (int i = start; i < pts.size(); ++i) {
        minY = qMin(minY, pts[i].y());
        maxY = qMax(maxY, pts[i].y());
    }
    // Диапазон оси ставится по min/max окна и пересчитывается на каждой
    // точке. Окно скользящее: одна точка входит справа, одна уходит
    // слева. На случайных числах из-за этого min и max меняются
    // постоянно — уехал максимум 49, стал 44, граница оси прыгнула
    // вниз; пришло 48 — прыгнула обратно. Кривая при этом та же, а
    // линейка под ней ползёт, и график визуально дёргается.
    //
    // У сигнала с постоянной амплитудой такого не будет: min и max
    // любого окна, покрывающего период, почти не меняются, и ось
    // встанет. Поэтому гистерезис или фиксация оси сейчас лечили бы
    // симптом — сначала сигнал, потом смотреть, осталось ли что
    // чинить.
    if (minY <= maxY) {
        double margin = qMax(1.0, (maxY - minY) * 0.1);
        m_axisY->setRange(minY - margin, maxY + margin);
    }

    // 2000 продублировано: и порог, и сколько отрезать. Поменяют одно,
    // забудут другое. Просится именованная константа рядом с m_window.
    if (m_series->count() > 2000) {
        m_series->removePoints(0, m_series->count() - 2000);
    }
}

void RealTimePlot::reset() {
    m_series->clear();
    m_t = 0;
    m_axisX->setRange(0, m_window);
    m_axisY->setRange(-10, 10);
}

void RealTimePlot::setCurveColor(const QColor& color) {
    QPen pen = m_series->pen();
    pen.setColor(color);
    m_series->setPen(pen);
    m_series->setColor(color);
}
