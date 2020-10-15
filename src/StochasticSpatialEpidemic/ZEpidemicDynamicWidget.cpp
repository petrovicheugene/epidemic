//============================================================
#include "ZEpidemicDynamicWidget.h"
#include "ZAbstractPopulation.h"
#include "ZStochasticHeterogeneousProcessCommon.h"

#include <QApplication>
#include <QChart>
#include <QChartView>
#include <QLegendMarker>
#include <QSplineSeries>
#include <QVBoxLayout>
//============================================================
ZEpidemicDynamicWidget::ZEpidemicDynamicWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
    zp_setPopulationSize(1);
    zp_setVisibleStepInChart(10);
    zv_pocessFinished = true;
    zv_clearChartAutomatically = false;
}
//============================================================
void ZEpidemicDynamicWidget::zh_createComponents()
{
    zv_chart = new QChart();

    QVBoxLayout* mainlayout = new QVBoxLayout;
    setLayout(mainlayout);

    zv_chartView = new QChartView;
    zv_chartView->setChart(zv_chart);
    zv_chartView->setFrameStyle(QFrame::Box | QFrame::Sunken);
    mainlayout->addWidget(zv_chartView);

    zv_chartView->setRenderHint(QPainter::Antialiasing);
    zv_chartView->chart()->setTitle("Epidemic dynamic");
}
//============================================================
void ZEpidemicDynamicWidget::zh_createConnections() {}
//============================================================
void ZEpidemicDynamicWidget::zp_updateCharts(QMap<QString, quint64> populationHealthStatus,
                                             bool lastInStep)
{
    if (!lastInStep)
    {
        return;
    }

    if (zv_pocessFinished)
    {
        zh_restartWorkSeries();
    }

    int populationSize = 0;
    QtCharts::QSplineSeries* cSeries;
    QMap<QString, quint64>::iterator it;
    for (it = populationHealthStatus.begin(); it != populationHealthStatus.end(); ++it)
    {
        cSeries = qobject_cast<QSplineSeries*>(zv_workSeries.value(it.key(), nullptr));
        populationSize += it.value();
        if (!cSeries)
        {
            cSeries = new QSplineSeries();
            cSeries->setName(it.key());

            QPen pen(healthStatusColor(it.key()));
            pen.setWidth(3);
            cSeries->setPen(pen);
            zv_chartView->chart()->addSeries(cSeries);
            zv_chartView->chart()->createDefaultAxes();
            zp_setVisibleStepInChart();
            zv_workSeries.insert(it.key(), cSeries);
        }

        int cCount = cSeries->points().count();
        *cSeries << QPointF(cCount, it.value());
    }

    zp_setPopulationSize(populationSize);
}
//============================================================
void ZEpidemicDynamicWidget::zp_finishProcess()
{
    zv_pocessFinished = true;
}
//============================================================
void ZEpidemicDynamicWidget::zh_restartWorkSeries()
{
    zv_pocessFinished = false;
    if (zv_clearChartAutomatically)
    {
        zp_clearCharts();
    }

    QtCharts::QSplineSeries* cSeries;
    QHash<QString, QtCharts::QAbstractSeries*>::iterator it;
    for (it = zv_workSeries.begin(); it != zv_workSeries.end(); ++it)
    {
        cSeries = qobject_cast<QSplineSeries*>(zv_workSeries.value(it.key(), nullptr));

        QPen pen(healthStatusColor(it.key()));
        pen.setWidth(1);
        cSeries->setPen(pen);
    }

    QList<QLegendMarker*> markers = zv_chart->legend()->markers();
    foreach (auto marker, markers)
    {
        marker->setVisible(false);
    }

    zv_workSeries.clear();
}
//============================================================
void ZEpidemicDynamicWidget::zp_clearCharts()
{
    zv_workSeries.clear();
    zv_chart->removeAllSeries();
}
//============================================================
void ZEpidemicDynamicWidget::zp_setPopulationSize(int size)
{
    if (size > 0)
    {
        zv_populationSize = size;
    }

    if (!zv_chartView->chart()->axes(Qt::Vertical).isEmpty())
    {
        zv_chartView->chart()->axes(Qt::Vertical).first()->setRange(0, zv_populationSize);
    }
}
//============================================================
void ZEpidemicDynamicWidget::zp_setVisibleStepInChart(int steps)
{
    if (steps > 0)
    {
        zv_visibleStepsInChart = steps;
    }

    if (!zv_chartView->chart()->axes(Qt::Horizontal).isEmpty())
    {
        zv_chartView->chart()->axes(Qt::Horizontal).first()->setRange(0, zv_visibleStepsInChart);
    }
}
//============================================================
void ZEpidemicDynamicWidget::zp_setClearChartsAutomaticallyFlag(bool flag)
{
    zv_clearChartAutomatically = flag;
}
//============================================================
