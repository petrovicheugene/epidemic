//============================================================
#include "MainWindow.h"

#include "ZAbstractEpidemicProcess.h"
#include "ZAbstractPopulation.h"
#include "ZEpidemicDynamicWidget.h"
#include "ZStochasticHeterogeneousSIRFactory.h"

#include "ZPositionedIndividualGraphicsItem.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSettings>
#include <QSplitter>
#include <QVBoxLayout>
//============================================================
MainWindow::MainWindow(QWidget* parent) : ZBaseMainWindow(parent)
{
    zv_population = nullptr;
    zv_epidemicProcess = nullptr;

    zv_populationWidget = nullptr;
    zv_groupDynamicChart = nullptr;

    zv_chartSplitter = nullptr;
    zv_epidemicProcessDashBoard = nullptr;
    zv_populationDashBoard = nullptr;
    zv_epidemicDynamicWidget = nullptr;

    zh_createComponents();
    zh_createConnections();

    zh_restoreSettings();
}
//============================================================
MainWindow::~MainWindow()
{
    zh_saveSettings();
}
//============================================================
void MainWindow::zh_createComponents()
{
    // components
    ZAbstractFactory* factory = zh_createFactory();
    if (!factory)
    {
        QString msg = tr("Cannot initialize the application. The factory is not found!");
        QMessageBox::critical(this, tr("App error"), msg, QMessageBox::Ok);
        return;
    }

    // Widgets
    // central widget
    QWidget* widget = new QWidget;
    QHBoxLayout* mainLayout = new QHBoxLayout;
    widget->setLayout(mainLayout);
    setCentralWidget(widget);

    // dashBoard
    QWidget* dashBoard = new QWidget(this);
    QVBoxLayout* dashBoardLayout = new QVBoxLayout;
    dashBoard->setLayout(dashBoardLayout);
    mainLayout->addWidget(dashBoard);

    // population dashboard
    QGroupBox* groupBox = new QGroupBox;
    QVBoxLayout* groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);
    dashBoardLayout->addWidget(groupBox);

    zv_populationDashBoard = factory->zp_createPopulationDashBoard();
    groupBoxLayout->addWidget(zv_populationDashBoard);

    // epidemic dashboard
    groupBox = new QGroupBox;
    groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);
    dashBoardLayout->addWidget(groupBox);

    zv_epidemicProcessDashBoard = factory->zp_createEpidemicProcessDashBoard();
    groupBoxLayout->addWidget(zv_epidemicProcessDashBoard);

    // charts
    zv_chartSplitter = new QSplitter(Qt::Vertical);
    zv_chartSplitter->setObjectName("ChartSplitter");
    mainLayout->addWidget(zv_chartSplitter, 99999);

    zv_populationWidget = factory->zp_createPopulationWidget();
    zv_chartSplitter->addWidget(zv_populationWidget);

    zv_epidemicDynamicWidget = new ZEpidemicDynamicWidget(this);
    zv_chartSplitter->addWidget(zv_epidemicDynamicWidget);


    // GRAPHICS
    //    QGraphicsScene* graphicsScene = new QGraphicsScene(this);
    //    QGraphicsView* view = new QGraphicsView(graphicsScene, this);
    //    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    //    zv_chartSplitter->addWidget(view);
    //    for (int i = 0; i < 10; ++i)
    //    {
    //        ZPositionedIndividualGraphicsItem* obj = new ZPositionedIndividualGraphicsItem;
    //        obj->setPos(i, i);
    //        graphicsScene->addItem(obj);
    //    }

    // components
    zv_population = factory->zp_createPopulation();
    zv_population->setParent(this);
    zv_epidemicProcess = factory->zp_createEpidemicProcess();
    zv_epidemicProcess->setParent(this);

    delete factory;
}
//============================================================
void MainWindow::zh_createConnections()
{
    if (zv_epidemicProcess && zv_population)
    {
        zv_epidemicProcess->zp_setPopulation(zv_population);
        zv_epidemicProcessDashBoard->zp_connect(zv_epidemicProcess);
        zv_populationDashBoard->zp_connect(zv_population);
    }

    if (zv_populationWidget && zv_population)
    {
        zv_populationWidget->zp_setPopulation(zv_population);
    }
}
//============================================================
ZAbstractFactory* MainWindow::zh_createFactory()
{
    return new ZStochasticHeterogeneousSIRFactory;
}
//============================================================
void MainWindow::zh_saveSettings() const
{
    ZBaseMainWindow::zh_saveSettings();
    QSettings settings;
    settings.beginGroup(this->metaObject()->className());
    settings.setValue(zv_chartSplitter->objectName(), zv_chartSplitter->saveState());
    while (!settings.group().isEmpty())
    {
        settings.endGroup();
    }
}
//============================================================
void MainWindow::zh_restoreSettings()
{
    ZBaseMainWindow::zh_restoreSettings();
    QSettings settings;
    settings.beginGroup(this->metaObject()->className());
    QVariant vData = settings.value(zv_chartSplitter->objectName());
    if (vData.isValid() && vData.canConvert<QByteArray>())
    {
        zv_chartSplitter->restoreState(vData.toByteArray());
    }

    while (!settings.group().isEmpty())
    {
        settings.endGroup();
    }
}
//============================================================
