//============================================================
#include "ZStochasticHeterogeneousSIRProcess.h"
#include "ZHeterogeneousPopulation.h"
//============================================================
ZStochasticHeterogeneousSIRProcess::ZStochasticHeterogeneousSIRProcess(QObject* parent)
    : ZAbstractEpidemicProcess(parent)
{
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zp_setPopulation(ZAbstractPopulation* population) {}
//============================================================
QStringList ZStochasticHeterogeneousSIRProcess::zp_groupList() const {}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zp_start() {}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zp_pause() {}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zp_stop() {}
//============================================================
