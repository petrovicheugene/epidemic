//============================================================
#ifndef ZSTOCHASTICHETEROGENEOUSSIRFACTORY_H
#define ZSTOCHASTICHETEROGENEOUSSIRFACTORY_H
/**
// The Factory of components for the Stochastic Spatial-Heterogeneous SIR Model
*/
//============================================================
#include "ZAbstractFactory.h"
//============================================================
class ZStochasticHeterogeneousSIRFactory : public ZAbstractFactory
{
    Q_OBJECT
public:
    explicit ZStochasticHeterogeneousSIRFactory(QObject *parent = nullptr);
    virtual ZAbstractPopulation* zp_createPopulation() const override;
    virtual ZAbstractEpidemicProcess* zp_createEpidemicProcess() const override;
    virtual ZAbstractDashBoard* zp_createPopulationDashBoard() const override;
    virtual ZAbstractDashBoard* zp_createEpidemicProcessDashBoard() const override;
};
//============================================================
#endif // ZSTOCHASTICHETEROGENEOUSSIRFACTORY_H
