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
#include <QToolButton>
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

    zv_chartSplitter = nullptr;
    zv_epidemicProcessDashBoard = nullptr;
    zv_populationDashBoard = nullptr;
    zv_epidemicDynamicWidget = nullptr;

    zh_createComponents();
    zh_createConnections();
    zh_createMenu();

    zh_restoreSettings();
    zh_updateStatusBarPopulationState();
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
    zv_populationDashBoard->setObjectName("PopulationDashBoard");
    dashBoardLayout->addWidget(zv_populationDashBoard);

    // epidemic dashboard
    zv_epidemicProcessDashBoard = factory->zp_createEpidemicProcessDashBoard();
    zv_epidemicProcessDashBoard->setObjectName("EpidemicProcessDashBoard");
    dashBoardLayout->addWidget(zv_epidemicProcessDashBoard);

    // charts
    zv_chartSplitter = new QSplitter(Qt::Vertical);
    zv_chartSplitter->setObjectName("ChartSplitter");
    zv_mainSplitter->addWidget(zv_chartSplitter);

    zv_populationWidget = factory->zp_createPopulationWidget();
    zv_populationWidget->setObjectName("populationWidget");
    zv_chartSplitter->addWidget(zv_populationWidget);

    zv_epidemicDynamicWidget = new ZEpidemicDynamicWidget(this);
    zv_epidemicDynamicWidget->setObjectName("EpidemicDynamicWidget");
    zv_chartSplitter->addWidget(zv_epidemicDynamicWidget);

    // components
    zv_population = factory->zp_createPopulation();
    zv_epidemicProcess = factory->zp_createEpidemicProcess();
    processThread = new QThread(this);
    zv_epidemicProcess->moveToThread(processThread);
    zv_population->moveToThread(processThread);
    processThread->start();

    delete factory;
    zv_populationSizeStatusBarLabel = new QLabel(this);
    zv_populationHealthStatusBarLabel = new QLabel(this);
    zv_epidemicDayLabel = new QLabel(this);

    statusBar()->addWidget(zv_populationSizeStatusBarLabel);
    statusBar()->addWidget(zv_epidemicDayLabel);
    statusBar()->addWidget(zv_populationHealthStatusBarLabel);
}
//============================================================
void MainWindow::zh_createConnections()
{
    zv_epidemicProcess->zp_setPopulation(zv_population);
    zv_epidemicProcessDashBoard->zp_connect(zv_epidemicProcess);
    zv_populationDashBoard->zp_connect(zv_population);

    zv_populationWidget->zp_setPopulation(zv_population);

    zv_populationDashBoard->zp_connect(zv_populationWidget);
    zv_populationDashBoard->zp_connect(zv_epidemicDynamicWidget);

    connect(zv_population,
            &ZAbstractPopulation::zg_populationStateChanged,
            this,
            &MainWindow::zh_updateStatusBarPopulationState);
    connect(zv_population,
            &ZAbstractPopulation::zg_populationOperation,
            this,
            &MainWindow::zh_updatePopulationOperation);

    connect(zv_epidemicProcess,
            &ZAbstractEpidemicProcess::zg_epidemicFinished,
            this,
            &MainWindow::zh_onEpidemicFinish);

    connect(zv_epidemicProcess,
            &ZAbstractEpidemicProcess::zg_epidemicStep,
            this,
            &MainWindow::zh_updateStatusBarEpidemicStep);

    connect(zv_epidemicProcess,
            &ZAbstractEpidemicProcess::zg_epidemicStep,
            zv_population,
            &ZAbstractPopulation::zp_notifyPopulationHealthStatus);

    // charts
    connect(zv_epidemicProcess,
            &ZAbstractEpidemicProcess::zg_epidemicFinished,
            zv_epidemicDynamicWidget,
            &ZEpidemicDynamicWidget::zp_finishProcess);

    connect(zv_population,
            &ZAbstractPopulation::zg_populationStateChanged,
            zv_epidemicDynamicWidget,
            &ZEpidemicDynamicWidget::zp_updateCharts);

    zv_epidemicProcessDashBoard->zp_connect(zv_epidemicDynamicWidget);
}
//============================================================
void MainWindow::zh_createMenu()
{
    QMenu* aboutMenu = new QMenu;
    aboutMenu->setIcon(QIcon(":/images/info.png"));
    aboutMenu->addAction(zv_aboutAction);
    aboutMenu->addAction(zv_aboutQtAction);

    menuBar()->addMenu(aboutMenu);
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
void MainWindow::zh_updateStatusBarPopulationState(QMap<QString, quint64> populationHealthStatus)
{
    QString msg;
    quint64 populationSize = 0;
    foreach (auto name, populationHealthStatus.keys())
    {
        msg += tr(" %1: %2. ").arg(name, QString::number(populationHealthStatus.value(name)));
        populationSize += populationHealthStatus.value(name);
    }

    zv_populationHealthStatusBarLabel->setText(msg);

    msg.clear();
    msg = tr("Total population size: %1. ").arg(QString::number(populationSize));
    zv_populationSizeStatusBarLabel->setText(msg);
}
//============================================================
void MainWindow::zh_updatePopulationOperation(int nOperation, QString auxMsg)
{
    ZAbstractPopulation::PopulationOperation operation
        = static_cast<ZAbstractPopulation::PopulationOperation>(nOperation);
    if (operation == ZAbstractPopulation::PO_GENERATING)
    {
        statusBar()->showMessage(tr("Generating of population. %1").arg(auxMsg));
    }

    else if (operation == ZAbstractPopulation::PO_REMOVING)
    {
        statusBar()->showMessage(tr("Removing of population. %1").arg(auxMsg));
    }
    else if (operation == ZAbstractPopulation::PO_READY)
    {
        statusBar()->clearMessage();
        statusBar()->showMessage(tr("Ready. %1").arg(auxMsg), 5000);
    }
    else
    {
        statusBar()->clearMessage();
    }
}
//============================================================
void MainWindow::zh_updateStatusBarEpidemicStep(QVariant data)
{
    if (!data.isValid() || !data.canConvert<int>())
    {
        return;
    }
    QString msg = tr("Epidemic day: %1. ").arg(QString::number(data.toInt()));
    zv_epidemicDayLabel->setText(msg);
}
//============================================================
void MainWindow::zh_onEpidemicFinish()
{
    statusBar()->showMessage(tr("Epidimic finished"), 3000);
}
//============================================================
