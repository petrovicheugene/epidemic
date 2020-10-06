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
class ZAbstractPopulationWidget;
class ZEpidemicDynamicWidget;

class QChart;
class QLabel;
class QSplitter;

//============================================================
class MainWindow : public ZBaseMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:

    void zg_inqueryPopulationHealthStatusReport(QMap<QString, quint64>& populationHealthStatus) const;
    void zg_inqueryPopulationSize(quint64& size) const;
    void zg_inqueryEpidemicStep(int& day) const;

private:
    // VARS
    QThread* processThread;

    ZAbstractPopulation* zv_population;
    ZAbstractEpidemicProcess* zv_epidemicProcess;

    ZAbstractPopulationWidget* zv_populationWidget;
    QChart* zv_groupDynamicChart;

    QSplitter* zv_mainSplitter;
    QSplitter* zv_chartSplitter;
    ZAbstractDashBoard* zv_epidemicProcessDashBoard;
    ZAbstractDashBoard* zv_populationDashBoard;
    ZEpidemicDynamicWidget* zv_epidemicDynamicWidget;
    QLabel* zv_populationSizeStatusBarLabel;
    QLabel* zv_populationHealthStatusBarLabel;

    //FUNCS

    void zh_createComponents();
    void zh_createConnections();
    ZAbstractFactory* zh_createFactory();
    void zh_saveSettings() const override;
    void zh_restoreSettings() override;

    void zh_onPopulationStateChange();
    void zh_onEpidemicFinish();
};
//============================================================
#endif // MAINWINDOW_H
