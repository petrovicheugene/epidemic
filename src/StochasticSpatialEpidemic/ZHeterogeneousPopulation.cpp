//============================================================
#include "ZHeterogeneousPopulation.h"

#include "ZPositionedIndividual.h"
#include "ZRandom2DPositionGenerator.h"
//============================================================
ZHeterogeneousPopulation::ZHeterogeneousPopulation(QObject* parent) : ZAbstractPopulation(parent) {}
//============================================================
void ZHeterogeneousPopulation::zp_generate()
{
    ZRandom2DPositionGenerator generator;
    QList<QPointF> positionList = generator.zp_generate(QPointF(0, 0), 1, 10);
    ZPositionedIndividual* individual;
    foreach (auto pos, positionList)
    {
        individual = new ZPositionedIndividual(pos, ZPositionedIndividual::HS_SUSCEPTIBLE, this);
        connect(individual,
                &ZPositionedIndividual::zg_healthStateChanged,
                this,
                &ZHeterogeneousPopulation::zg_individualHealthChanged);
        connect(individual,
                &ZPositionedIndividual::zg_positionChanged,
                this,
                &ZHeterogeneousPopulation::zg_individualPositionChanged);
        zv_individuals.insert(individual->zp_id(), individual);
        emit zg_individualAdded(individual->zp_id());
    }
}
//============================================================
void ZHeterogeneousPopulation::zp_individualPosition(quint64 id, QPointF& position) const
{
    ZPositionedIndividual* individual = zv_individuals.value(id, nullptr);
    if (!individual)
    {
        position = QPointF();
        return;
    }

    position = individual->zp_position();
}
//============================================================
void ZHeterogeneousPopulation::zp_individualHealthState(quint64 id, int& healthState) const
{
    ZPositionedIndividual* individual = zv_individuals.value(id, nullptr);
    if (!individual)
    {
        healthState = ZPositionedIndividual::HS_NOT_DEFINED;
        return;
    }

    healthState = individual->zp_healthState();
}
//============================================================
void ZHeterogeneousPopulation::zp_setIndividualHealth(quint64 id, int healthState)
{
    ZPositionedIndividual* individual = zv_individuals.value(id, nullptr);
    if (!individual)
    {
        healthState = ZPositionedIndividual::HS_NOT_DEFINED;
        return;
    }

    ZPositionedIndividual::HealthState state = static_cast<ZPositionedIndividual::HealthState>(
        healthState);
    individual->zp_setHealthState(state);
}
//============================================================
