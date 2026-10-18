#pragma once

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <QColor>

// График «бегущей ленты»: серия уезжает вправо, ось X едет за ней,
// ось Y подстраивается под последние m_window точек.
// Пространства имён у Qt здесь нет: твой Qt собран без QT_NAMESPACE,
// поэтому QChartView/QLineSeries/QValueAxis лежат в глобальном.
// Если Qt пересобрать с QT_NAMESPACE, ко всему QtCharts-ному
// придётся дописать префикс QtCharts::.
class RealTimePlot : public QChartView {
    Q_OBJECT

public:
    explicit RealTimePlot(QWidget *parent = nullptr);

    // Добавить новую точку и сдвинуть окно просмотра.
    void addPoint(double y);

    // Очистить график (например, перед новым подключением).
    void reset();

    // Цвет кривой меняется на лету: берём текущее перо и меняем только цвет,
    // поэтому толщина (если её зададут отдельно) сохраняется.
    void setCurveColor(const QColor &color);

private:
    QLineSeries *m_series = nullptr;
    QValueAxis  *m_axisX  = nullptr;
    QValueAxis  *m_axisY  = nullptr;

    double m_t      = 0;      // текущий X (счётчик точек)
    int    m_window = 20;     // сколько точек видно одновременно
};