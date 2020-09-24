//============================================================
#include "ZEpidemicDynamicWidget.h"
#include <QApplication>
#include <QChartView>
#include <QLegendMarker>
#include <QScatterSeries>
#include <QVBoxLayout>
//============================================================
ZEpidemicDynamicWidget::ZEpidemicDynamicWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//============================================================
void ZEpidemicDynamicWidget::zh_createComponents()
{
    QVBoxLayout* mainlayout = new QVBoxLayout;
    setLayout(mainlayout);

    zv_chartView = new QChartView;
    mainlayout->addWidget(zv_chartView);
}
//============================================================
void ZEpidemicDynamicWidget::zh_createConnections() {}
//============================================================
