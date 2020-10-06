//============================================================
#include "ZHeterogeneousPopulation.h"

#include "ZDistanceRepository.h"
#include "ZPositionedIndividual.h"
#include "ZRandom2DPositionGenerator.h"

#include <omp.h>
#include <QDebug>
#include <QElapsedTimer>
//============================================================
ZHeterogeneousPopulation::ZHeterogeneousPopulation(QObject* parent) : ZAbstractPopulation(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//============================================================
void ZHeterogeneousPopulation::zh_createComponents()
{
    zv_distanceRepository = new ZDistanceRepository(this);
}
//============================================================
void ZHeterogeneousPopulation::zh_createConnections()
{
    connect(this,
            &ZHeterogeneousPopulation::zg_individualHealthChanged,
            this,
            &ZHeterogeneousPopulation::zg_populationStateChanged);
    connect(this,
            &ZHeterogeneousPopulation::zg_individualPositionChanged,
            this,
            &ZHeterogeneousPopulation::zg_populationStateChanged);
    connect(this,
            &ZHeterogeneousPopulation::zg_individualAdded,
            this,
            &ZHeterogeneousPopulation::zg_populationStateChanged);
    connect(this,
            &ZHeterogeneousPopulation::zg_individualRemoved,
            this,
            &ZHeterogeneousPopulation::zg_populationStateChanged);

    connect(zv_distanceRepository,
            &ZDistanceRepository::zg_inquirePositionForId,
            this,
            &ZHeterogeneousPopulation::zp_individualPosition);
}
//============================================================
void ZHeterogeneousPopulation::zp_generate(QVariant vSettings)
{
    if (!vSettings.isValid() || !vSettings.canConvert<ZGenerationSettings>())
    {
        return;
    }

    QElapsedTimer timer;
    timer.start();

    ZGenerationSettings settings = vSettings.value<ZGenerationSettings>();

    ZRandom2DPositionGenerator generator;
    QList<QPointF> positionList = generator.zp_generate(settings);

    qDebug() << "GENERATION ELAPSED" << timer.elapsed();
    timer.restart();
#pragma omp parallel
    {
        ZPositionedIndividual* individual;
        // foreach (auto pos, positionList)
#pragma omp for

        for (int i = 0; i < positionList.count(); ++i)
        {
            QPointF pos = positionList.at(i);
            individual = new ZPositionedIndividual;
            connect(individual,
                    &ZPositionedIndividual::zg_healthStateChanged,
                    this,
                    &ZHeterogeneousPopulation::zg_individualHealthChanged);
            connect(individual,
                    &ZPositionedIndividual::zg_positionChanged,
                    this,
                    &ZHeterogeneousPopulation::zg_individualPositionChanged);
            connect(individual,
                    &ZPositionedIndividual::zg_positionChanged,
                    zv_distanceRepository,
                    &ZDistanceRepository::zp_onPositionChange);

#pragma omp critical
            {
                zv_individuals.insert(individual->zp_id(), individual);
            }
            emit zg_individualAdded(individual->zp_id());

            individual->zp_setHealthState(HS_SUSCEPTIBLE);
            individual->zp_setPosition(pos);
        }
    }
    //    ZPositionedIndividual* individual;
    //    // foreach (auto pos, positionList)
    //    for (int i = 0; i < positionList.count(); ++i)
    //    {
    //        QPointF pos = positionList.at(i);
    //        individual = new ZPositionedIndividual(this);
    //        connect(individual,
    //                &ZPositionedIndividual::zg_healthStateChanged,
    //                this,
    //                &ZHeterogeneousPopulation::zg_individualHealthChanged);
    //        connect(individual,
    //                &ZPositionedIndividual::zg_positionChanged,
    //                this,
    //                &ZHeterogeneousPopulation::zg_individualPositionChanged);
    //        connect(individual,
    //                &ZPositionedIndividual::zg_positionChanged,
    //                zv_distanceRepository,
    //                &ZDistanceRepository::zp_onPositionChange);

    //        zv_individuals.insert(individual->zp_id(), individual);
    //        emit zg_individualAdded(individual->zp_id());

    //        individual->zp_setHealthState(HS_SUSCEPTIBLE);
    //        individual->zp_setPosition(pos);
    //    }

    qDebug() << "INDIVIDUALS CREATION ELAPSED" << timer.elapsed();
}
//============================================================
void ZHeterogeneousPopulation::zp_clear()
{
    QList<quint64> idList = zv_individuals.keys();
    qDeleteAll(zv_individuals);
    zv_individuals.clear();

    foreach (auto id, idList)
    {
        emit zg_individualRemoved(id);
    }
}
//============================================================
void ZHeterogeneousPopulation::zp_setHealthStatus(HealthStatus heathStatus)
{
    foreach (auto individual, zv_individuals)
    {
        individual->zp_setHealthState(heathStatus);
    }
}
//============================================================
void ZHeterogeneousPopulation::zp_individualPosition(quint64 id, QPointF& position, bool* ok) const
{
    ZPositionedIndividual* individual = zv_individuals.value(id, nullptr);
    if (!individual)
    {
        position = QPointF();
        if (ok)
        {
            *ok = false;
        }
        return;
    }

    position = individual->zp_position();

    if (ok)
    {
        *ok = true;
    }
}
//============================================================
void ZHeterogeneousPopulation::zp_healthStateForId(quint64 id,
                                                   HealthStatus& healthState,
                                                   bool* ok) const
{
    ZPositionedIndividual* individual = zv_individuals.value(id, nullptr);
    if (!individual)
    {
        healthState = HS_NOT_DEFINED;
        if (ok)
        {
            *ok = false;
        }
        return;
    }

    healthState = individual->zp_healthState();

    if (ok)
    {
        *ok = true;
    }
}
//============================================================
void ZHeterogeneousPopulation::zp_setHealthStateForId(quint64 id, int healthState, bool* ok)
{
    ZPositionedIndividual* individual = zv_individuals.value(id, nullptr);
    if (!individual)
    {
        if (ok)
        {
            *ok = false;
        }
        return;
    }

    HealthStatus state = static_cast<HealthStatus>(healthState);
    individual->zp_setHealthState(state);

    if (ok)
    {
        *ok = true;
    }
}
//============================================================
void ZHeterogeneousPopulation::zp_idListForHealthState(HealthStatus healthState,
                                                       QList<quint64>& idList) const
{
    idList.clear();
    QMap<quint64, ZPositionedIndividual*>::const_iterator it;
    for (it = zv_individuals.begin(); it != zv_individuals.end(); ++it)
    {
        if (it.value()->zp_healthState() == healthState)
        {
            idList.append(it.key());
        }
    }
}
//============================================================
void ZHeterogeneousPopulation::zp_recoveryProbabilityForId(quint64 id, qreal& probability, bool* ok)
{
    ZPositionedIndividual* individual = zv_individuals.value(id, nullptr);
    if (!individual)
    {
        if (ok)
        {
            *ok = false;
        }
        return;
    }

    probability = individual->zp_recoveryProbability();

    if (ok)
    {
        *ok = true;
    }
}
//============================================================
void ZHeterogeneousPopulation::zp_setRecoveryProbabilityForId(quint64 id,
                                                              qreal probability,
                                                              bool* ok)
{
    ZPositionedIndividual* individual = zv_individuals.value(id, nullptr);
    if (!individual)
    {
        if (ok)
        {
            *ok = false;
        }
        return;
    }

    individual->zp_setRecoveryProbability(probability);

    if (ok)
    {
        *ok = true;
    }
}
//============================================================
void ZHeterogeneousPopulation::zp_idForIndex(int index, quint64& id, bool* ok) const
{
    if (index < 0 || index >= zv_individuals.count())
    {
        if (ok)
        {
            *ok = false;
        }
        return;
    }

    id = zv_individuals.keys().at(index);
    if (ok)
    {
        *ok = true;
    }
    return;
}
//============================================================
void ZHeterogeneousPopulation::zp_distansesForId(quint64 id,
                                                 QMap<quint64, qreal>& distanceMap,
                                                 bool* ok) const
{
    if (!zv_individuals.keys().contains(id))
    {
        if (ok)
        {
            *ok = false;
        }
        return;
    }

    zv_distanceRepository->zp_distancesForId(id, distanceMap);
    if (ok)
    {
        *ok = true;
    }
    return;
}
//============================================================
void ZHeterogeneousPopulation::zp_populationSize(quint64& size) const
{
    size = zv_individuals.count();
}
//============================================================
void ZHeterogeneousPopulation::zp_populationHealthStatus(
    QMap<int, quint64>& populationHealthStatus) const
{
    populationHealthStatus.clear();
    populationHealthStatus.insert(HS_SUSCEPTIBLE, zp_numberForHealthStatus(HS_SUSCEPTIBLE));
    populationHealthStatus.insert(HS_INFECTIOUS, zp_numberForHealthStatus(HS_INFECTIOUS));
    populationHealthStatus.insert(HS_RECOVERED, zp_numberForHealthStatus(HS_RECOVERED));
}
//============================================================
void ZHeterogeneousPopulation::zp_populationHealthStatusReport(
    QMap<QString, quint64>& populationHealthStatus) const
{
    populationHealthStatus.clear();
    populationHealthStatus.insert(healthStatusName(HS_SUSCEPTIBLE),
                                  zp_numberForHealthStatus(HS_SUSCEPTIBLE));
    populationHealthStatus.insert(healthStatusName(HS_INFECTIOUS),
                                  zp_numberForHealthStatus(HS_INFECTIOUS));
    populationHealthStatus.insert(healthStatusName(HS_RECOVERED),
                                  zp_numberForHealthStatus(HS_RECOVERED));
}

//============================================================
quint64 ZHeterogeneousPopulation::zp_numberForHealthStatus(int healthStatus) const
{
    quint64 count = 0;
    QMap<quint64, ZPositionedIndividual*>::const_iterator it;
    for (it = zv_individuals.begin(); it != zv_individuals.end(); ++it)
    {
        if (it.value()->zp_healthState() == healthStatus)
        {
            count++;
        }
    }

    return count;
}
//============================================================
