//============================================================
#ifndef ZGROUPDYNAMICWIDGET_H
#define ZGROUPDYNAMICWIDGET_H
//============================================================
#include <QWidget>
#include <QtCharts>
//============================================================
using namespace QtCharts;
//============================================================
class ZEpidemicDynamicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZEpidemicDynamicWidget(QWidget *parent = nullptr);

signals:

private:
    // VARS
    QChartView* zv_chartView;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
};
//============================================================
#endif // ZGROUPDYNAMICWIDGET_H
