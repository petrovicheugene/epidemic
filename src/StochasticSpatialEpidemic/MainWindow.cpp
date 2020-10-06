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
#include <QThread>
#include <QVBoxLayout>
//============================================================
MainWindow::MainWindow(QWidget* parent) : ZBaseMainWindow(parent)
{
    qRegisterMetaType<HealthStatus>("HealthStatus");
    qRegisterMetaType<int>("int");
    qRegisterMetaType<QPointF>("QPointF");

    processThread = nullptr;
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

    if (processThread)
    {
        processThread->terminate();
        processThread->wait(3000);
    }
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
    zv_mainSplitter = new QSplitter(Qt::Horizontal);
    zv_mainSplitter->setObjectName("MainSplitter");

    setCentralWidget(zv_mainSplitter);

    // dashBoard
    QWidget* dashBoard = new QWidget(this);
    QVBoxLayout* dashBoardLayout = new QVBoxLayout;
    dashBoard->setLayout(dashBoardLayout);
    zv_mainSplitter->addWidget(dashBoard);

    // population dashboard
    zv_populationDashBoard = factory->zp_createPopulationDashBoard();
    dashBoardLayout->addWidget(zv_populationDashBoard);

    // epidemic dashboard
    zv_epidemicProcessDashBoard = factory->zp_createEpidemicProcessDashBoard();
    dashBoardLayout->addWidget(zv_epidemicProcessDashBoard);

    // charts
    zv_chartSplitter = new QSplitter(Qt::Vertical);
    zv_chartSplitter->setObjectName("ChartSplitter");
    zv_mainSplitter->addWidget(zv_chartSplitter);

    zv_populationWidget = factory->zp_createPopulationWidget();
    zv_chartSplitter->addWidget(zv_populationWidget);

    zv_epidemicDynamicWidget = new ZEpidemicDynamicWidget(this);
    zv_chartSplitter->addWidget(zv_epidemicDynamicWidget);

    // components
    zv_population = factory->zp_createPopulation();
    // zv_population->setParent(this);
    zv_epidemicProcess = factory->zp_createEpidemicProcess();
    // zv_epidemicProcess->setParent(this);
    processThread = new QThread(this);
    zv_epidemicProcess->moveToThread(processThread);
    zv_population->moveToThread(processThread);
    processThread->start();

    delete factory;
    zv_populationSizeStatusBarLabel = new QLabel(this);
    zv_populationHealthStatusBarLabel = new QLabel(this);

    statusBar()->addWidget(zv_populationSizeStatusBarLabel);
    statusBar()->addWidget(zv_populationHealthStatusBarLabel);
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

    if (zv_populationWidget && zv_populationDashBoard)
    {
        zv_populationDashBoard->zp_connect(zv_populationWidget);
    }

    connect(zv_population,
            &ZAbstractPopulation::zg_populationStateChanged,
            this,
            &MainWindow::zh_onPopulationStateChange);

    connect(this,
            &MainWindow::zg_inqueryPopulationSize,
            zv_population,
            &ZAbstractPopulation::zp_populationSize);
    connect(this,
            &MainWindow::zg_inqueryPopulationHealthStatusReport,
            zv_population,
            &ZAbstractPopulation::zp_populationHealthStatusReport);

    connect(zv_epidemicProcess,
            &ZAbstractEpidemicProcess::zg_epidemicFinished,
            this,
            &MainWindow::zh_onEpidemicFinish);
    connect(this,
            &MainWindow::zg_inqueryEpidemicStep,
            zv_epidemicProcess,
            &ZAbstractEpidemicProcess::zp_processStep);
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
    settings.setValue(zv_mainSplitter->objectName(), zv_mainSplitter->saveState());
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

    vData = settings.value(zv_mainSplitter->objectName());
    if (vData.isValid() && vData.canConvert<QByteArray>())
    {
        zv_mainSplitter->restoreState(vData.toByteArray());
    }

    while (!settings.group().isEmpty())
    {
        settings.endGroup();
    }
}
//============================================================
void MainWindow::zh_onPopulationStateChange()
{
    quint64 populationSize;
    emit zg_inqueryPopulationSize(populationSize);
    QMap<QString, quint64> populationHealthStatus;
    emit zg_inqueryPopulationHealthStatusReport(populationHealthStatus);
    int day = 0;
    emit zg_inqueryEpidemicStep(day);

    QString msg = tr("Total population size: %1.").arg(QString::number(populationSize));
    zv_populationSizeStatusBarLabel->setText(msg);

    msg.clear();
    msg = tr("Epidemic day: %1.").arg(QString::number(day));
    foreach (auto name, populationHealthStatus.keys())
    {
        msg += tr(" %1: %2.").arg(name, QString::number(populationHealthStatus.value(name)));
    }

    zv_populationHealthStatusBarLabel->setText(msg);
}
//============================================================
void MainWindow::zh_onEpidemicFinish()
{
    statusBar()->showMessage(tr("Epidimic finished"), 3000);
}
//============================================================
