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
    qRegisterMetaType<QList<std::tuple<quint64, QPointF, HealthStatus>>>(
        "QList<std::tuple<quint64, QPointF, HealthStatus>>");
    qRegisterMetaType<QMap<QString, quint64>>("QMap<QString, quint64>");

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
            &ZHeterogeneousPopulation::zg_populationStateChangeNotification);
    connect(this,
            &ZHeterogeneousPopulation::zg_individualPositionChanged,
            this,
            &ZHeterogeneousPopulation::zg_populationStateChangeNotification);
    connect(this,
            &ZHeterogeneousPopulation::zg_individualAdded,
            this,
            &ZHeterogeneousPopulation::zg_populationStateChangeNotification);
    connect(this,
            &ZHeterogeneousPopulation::zg_individualListAdded,
            this,
            &ZHeterogeneousPopulation::zg_populationStateChangeNotification);
    connect(this,
            &ZHeterogeneousPopulation::zg_individualRemoved,
            this,
            &ZHeterogeneousPopulation::zg_populationStateChangeNotification);
    connect(this,
            &ZHeterogeneousPopulation::zg_populationStateChangeNotification,
            this,
            &ZHeterogeneousPopulation::zh_onPopulationStateChange);

    connect(zv_distanceRepository,
            &ZDistanceRepository::zg_inquirePositionForId,
            this,
            &ZHeterogeneousPopulation::zp_individualPosition);
    connect(zv_distanceRepository,
            &ZDistanceRepository::zg_inquireIndividualPositionMap,
            this,
            &ZHeterogeneousPopulation::zp_individualPositionMap);

    connect(this,
            &ZHeterogeneousPopulation::zg_individualRemoved,
            zv_distanceRepository,
            &ZDistanceRepository::zp_removeDistancesForId);
    connect(this,
            &ZHeterogeneousPopulation::zg_allIindividualsRemoved,
            zv_distanceRepository,
            &ZDistanceRepository::zp_clearDistances);
}
//============================================================
void ZHeterogeneousPopulation::zp_generate(QVariant vSettings)
{
    if (!vSettings.isValid() || !vSettings.canConvert<ZGenerationSettings>())
    {
        return;
    }

    emit zg_populationOperation(PO_GENERATING);
    ZGenerationSettings settings = vSettings.value<ZGenerationSettings>();

    ZRandom2DPositionGenerator generator;
    QList<QPointF> positionList = generator.zp_generate(settings);

    QList<std::tuple<quint64, QPointF, HealthStatus>> individualSpecList;
    ZPositionedIndividual* individual;
    QList<quint64> idList;
    int i;
    QMap<quint64, QPointF> positionMap;
#pragma omp parallel shared(positionMap, idList, individualSpecList) private(i, individual)
    {
#pragma omp for
        for (i = 0; i < positionList.count(); ++i)
        {
            QPointF pos = positionList.at(i);
            individual = new ZPositionedIndividual;
            individual->zp_setHealthState(HS_SUSCEPTIBLE);
            individual->zp_setPosition(pos);

            std::tuple<quint64, QPointF, HealthStatus> tuple(individual->zp_id(),
                                                             pos,
                                                             HS_SUSCEPTIBLE);
#pragma omp critical
            {
                individualSpecList.append(tuple);
            }

#pragma omp critical
            {
                idList.append(individual->zp_id());
            }

#pragma omp critical
            {
                zv_individuals.insert(individual->zp_id(), individual);
            }
#pragma omp critical
            {
                positionMap.insert(individual->zp_id(), pos);
            }
        }

#pragma omp barrier
    }

    emit zg_individualListAdded(individualSpecList);

    // connect each new individual
    for (int i = 0; i < idList.count(); ++i)
    {
        quint64 id = idList.at(i);

        individual = zv_individuals.value(id);
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
        if (i % 50 == 0)
        {
            emit zg_populationOperation(PO_GENERATING, QString::number(i));
        }

        emit zg_individualAdded(id);
        //individual->zp_notifyPosition();
    }

    emit zg_populationOperation(PO_GENERATING, tr("Recalculating distances"));
    QElapsedTimer timer;
    timer.start();
    zv_distanceRepository->zp_recalcSortedDistanceMap();
    emit zg_populationOperation(PO_READY,
                                tr(" Elapsed %1 s.").arg(QString::number(timer.elapsed() / 1000)));
}
//============================================================
void ZHeterogeneousPopulation::zp_clear()
{
    emit zg_populationOperation(PO_REMOVING);

    QList<quint64> idList = zv_individuals.keys();
    qDeleteAll(zv_individuals);
    zv_individuals.clear();

    emit zg_allIindividualsRemoved();

    foreach (auto id, idList)
    {
        emit zg_individualRemoved(id);
    }

    emit zg_populationOperation(PO_READY);
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
void ZHeterogeneousPopulation::zp_individualPositionMap(QMap<quint64, QPointF>& positionMap) const
{
    positionMap.clear();
    foreach (auto individual, zv_individuals)
    {
        positionMap.insert(individual->zp_id(), individual->zp_position());
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
void ZHeterogeneousPopulation::zp_setHealthStateForId(quint64 id, HealthStatus healthState, bool* ok)
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
void ZHeterogeneousPopulation::zp_sortedDistansesForId(quint64 id,
                                                       QMultiMap<qreal, quint64>& distanceMap,
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

    zv_distanceRepository->zp_sortedDistancesForId(id, distanceMap);
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
void ZHeterogeneousPopulation::zh_onPopulationStateChange() const
{
    QMap<QString, quint64> populationHealthStatus;
    zp_populationHealthStatusReport(populationHealthStatus);
    emit zg_populationStateChanged(populationHealthStatus);
}
//============================================================
