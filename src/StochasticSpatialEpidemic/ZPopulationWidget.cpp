//============================================================
#include "ZPopulationWidget.h"
#include <QApplication>
#include <QChartView>
#include <QLegendMarker>
#include <QScatterSeries>
#include <QVBoxLayout>
//============================================================
ZPopulationWidget::ZPopulationWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//============================================================
void ZPopulationWidget::zh_createComponents()
{
    QVBoxLayout* mainlayout = new QVBoxLayout;
    setLayout(mainlayout);

    zv_chartView = new QChartView;
    mainlayout->addWidget(zv_chartView);

}
//============================================================
void ZPopulationWidget::zh_createConnections()
{

}
//============================================================
