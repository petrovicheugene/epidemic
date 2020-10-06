//============================================================
#include "ZStochasticHeterogeneousSIRFactory.h"
#include "ZHeterogeneousPopulation.h"
#include "ZHeterogeneousPopulationDashBoard.h"
#include "ZHeterogeneousPopulationScene.h"
#include "ZHeterogeneousPopulationWidget.h"
#include "ZStochasticHeterogeneousSIRProcess.h"
#include "ZStochasticHeterogeneousSIRProcessDashBoard.h"
//============================================================
ZStochasticHeterogeneousSIRFactory::ZStochasticHeterogeneousSIRFactory(QObject* parent)
    : ZAbstractFactory(parent)
{
}
//============================================================
ZAbstractPopulation* ZStochasticHeterogeneousSIRFactory::zp_createPopulation() const
{
    return new ZHeterogeneousPopulation;
}
//============================================================
ZAbstractEpidemicProcess* ZStochasticHeterogeneousSIRFactory::zp_createEpidemicProcess() const
{
    return new ZStochasticHeterogeneousSIRProcess;
}
//============================================================
ZAbstractDashBoard* ZStochasticHeterogeneousSIRFactory::zp_createPopulationDashBoard() const
{
    return new ZHeterogeneousPopulationDashBoard;
}
//============================================================
ZAbstractDashBoard* ZStochasticHeterogeneousSIRFactory::zp_createEpidemicProcessDashBoard() const
{
    return new ZStochasticHeterogeneousSIRProcessDashBoard;
}
//============================================================
ZAbstractPopulationWidget* ZStochasticHeterogeneousSIRFactory::zp_createPopulationWidget() const
{
    return new ZHeterogeneousPopulationWidget;
}
//============================================================
