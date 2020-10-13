//============================================================
#ifndef ZGROUPDYNAMICWIDGET_H
#define ZGROUPDYNAMICWIDGET_H
//============================================================
#include <QHash>
#include <QWidget>
#include <QtCharts>
//============================================================
class ZAbstractEpidemicProcess;
class ZAbstractPopulation;
class QAbstractSeries;
//============================================================
//using namespace QtCharts;
//============================================================
class ZEpidemicDynamicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZEpidemicDynamicWidget(QWidget* parent = nullptr);

public slots:

    void zp_updateCharts(QMap<QString, quint64> populationHealthStatus, bool lastInStep);
    void zp_finishProcess();
    void zp_clearCharts();
    void zp_setPopulationSize(int size = 0);
    void zp_setVisibleStepInChart(int steps = 0);
    void zp_setClearChartsAutomaticallyFlag(bool flag);

signals:

private:
    // VARS
    QtCharts::QChartView* zv_chartView;
    QChart* zv_chart;
    QHash<QString, QtCharts::QAbstractSeries*> zv_workSeries;
    qreal zv_populationSize;
    qreal zv_visibleStepsInChart;
    bool zv_pocessFinished;
    bool zv_clearChartAutomatically;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_restartWorkSeries();
};
//============================================================
#endif // ZGROUPDYNAMICWIDGET_H
