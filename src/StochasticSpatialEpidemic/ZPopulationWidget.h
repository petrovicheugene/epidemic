//============================================================
#ifndef ZPOPULATIONWIDGET_H
#define ZPOPULATIONWIDGET_H
//============================================================
#include <QWidget>
#include <QtCharts>
//============================================================
using namespace QtCharts;
//============================================================
class ZPopulationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZPopulationWidget(QWidget *parent = nullptr);

signals:

private:

    // VARS
    QChartView* zv_chartView;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

};
//============================================================
#endif // ZPOPULATIONWIDGET_H