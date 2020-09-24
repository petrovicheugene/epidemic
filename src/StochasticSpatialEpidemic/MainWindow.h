//============================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//============================================================
#include <ZBaseMainWindow.h>
//============================================================
class ZAbstractEpidemicProcess;
class ZAbstractDashBoard;
class ZAbstractFactory;
class ZAbstractPopulation;
class ZEpidemicDynamicWidget;
class ZPopulationWidget;

class QChart;
class QSplitter;

//============================================================
class MainWindow : public ZBaseMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    // VARS
    ZAbstractPopulation* zv_population;
    ZAbstractEpidemicProcess* zv_epidemicProcess;

    QChart* zv_populationChart;
    QChart* zv_groupDynamicChart;

    QSplitter* zv_chartSplitter;
    ZAbstractDashBoard* zv_epidemicProcessDashBoard;
    ZAbstractDashBoard* zv_populationDashBoard;
    ZEpidemicDynamicWidget* zv_epidemicDynamicWidget;
    ZPopulationWidget* zv_populationWidget;

    //FUNCS

    void zh_createComponents();
    void zh_createConnections();
    ZAbstractFactory* zh_createFactory();
    void zh_saveSettings() const override;
    void zh_restoreSettings() override;
};
//============================================================
#endif // MAINWINDOW_H
