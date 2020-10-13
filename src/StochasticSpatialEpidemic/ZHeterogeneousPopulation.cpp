//============================================================
#include "ZHeterogeneousPopulation.h"

#include "ZDistanceRepository.h"
#include "ZPositionedIndividual.h"
#include "ZRandom2DPositionGenerator.h"

#include <omp.h>
#include <QDateTime>
#include <QDebug>
#include <QElapsedTimer>
//============================================================
ZHeterogeneousPopulation::ZHeterogeneousPopulation(QObject* parent) : ZAbstractPopulation(parent)
{
    qRegisterMetaType<QList<std::tuple<quint64, QPointF, HealthStatus>>>(
        "QList<std::tuple<quint64, QPointF, HealthStatus>>");
    qRegisterMetaType<QMap<QString, quint64>>("QMap<QString, quint64>");
    qRegisterMetaType<QHash<QString, quint64>>("QMap<QString, quint64>");

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
            &ZDistanceRepository::zg_inquireIndividualPositionHash,
            this,
            &ZHeterogeneousPopulation::zp_individualPositionHash);

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

    qint64 startTimeMark = QDateTime::currentMSecsSinceEpoch();

    emit zg_populationOperation(PO_GENERATING, tr("Creating individuals"));
    ZGenerationSettings settings = vSettings.value<ZGenerationSettings>();

    ZRandom2DPositionGenerator generator;
    QList<QPointF> positionList = generator.zp_generate(settings);

    QList<std::tuple<quint64, QPointF, HealthStatus>> individualSpecList;
    ZPositionedIndividual* individual;
    int i;
    QHash<quint64, QPointF> positionHash;
    QPointF pos;

    //#pragma omp parallel shared(positionHash, individualSpecList) private(i, individual, pos)
    {
        //#pragma omp for
        for (i = 0; i < positionList.count(); ++i)
        {
            pos = positionList.at(i);
            individual = new ZPositionedIndividual;
            individual->zp_setHealthState(HS_SUSCEPTIBLE);
            individual->zp_setPosition(pos);

            std::tuple<quint64, QPointF, HealthStatus> tuple(individual->zp_id(),
                                                             pos,
                                                             HS_SUSCEPTIBLE);
            //#pragma omp critical
            {
                individualSpecList.append(tuple);
            }
            //#pragma omp critical
            {
                zv_individualHash.insert(individual->zp_id(), individual);
            }
            //#pragma omp critical
            {
                positionHash.insert(individual->zp_id(), pos);
            }

            //#pragma omp master
            {
                connect(individual,
                        &ZPositionedIndividual::zg_healthStateChanged,
                        this,
                        &ZHeterogeneousPopulation::zp_individualHealthChanged);
                connect(individual,
                        &ZPositionedIndividual::zg_positionChanged,
                        this,
                        &ZHeterogeneousPopulation::zg_individualPositionChanged);
                connect(individual,
                        &ZPositionedIndividual::zg_positionChanged,
                        zv_distanceRepository,
                        &ZDistanceRepository::zp_recalcDistancesForPosition);

                emit zg_individualAdded(individual->zp_id());
            }
        }
    }

    emit zg_populationOperation(PO_GENERATING, tr("Displaying"));
    emit zg_individualListAdded(individualSpecList);

    emit zg_populationOperation(PO_GENERATING, tr("Recalculating distances"));
    zv_distanceRepository->zp_recalcDistancesForPositions(positionHash);

    qint64 endTimeMark = QDateTime::currentMSecsSinceEpoch();
    qint64 elapsedSec = (endTimeMark - startTimeMark) / 1000;

    emit zg_populationOperation(PO_READY,
                                tr(" Elapsed time: %1 m. %2 s.")
                                    .arg(QString::number(elapsedSec / 60),
                                         QString::number(elapsedSec % 60)));

    return;
}
//============================================================
void ZHeterogeneousPopulation::zp_clear()
{
    emit zg_populationOperation(PO_REMOVING);

    QList<quint64> idList = zv_individualHash.keys();
    qDeleteAll(zv_individualHash);
    zv_individualHash.clear();

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
    foreach (auto individual, zv_individualHash)
    {
        individual->zp_setHealthState(heathStatus);
    }
}
//============================================================
void ZHeterogeneousPopulation::zp_individualPosition(quint64 id, QPointF& position, bool* ok) const
{
    ZPositionedIndividual* individual = zv_individualHash.value(id, nullptr);
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
void ZHeterogeneousPopulation::zp_individualPositionHash(QHash<quint64, QPointF>& positionHash) const
{
    positionHash.clear();
    foreach (auto individual, zv_individualHash)
    {
        positionHash.insert(individual->zp_id(), individual->zp_position());
    }
}
//============================================================
void ZHeterogeneousPopulation::zp_healthStateForId(quint64 id,
                                                   HealthStatus& healthState,
                                                   bool* ok) const
{
    ZPositionedIndividual* individual = zv_individualHash.value(id, nullptr);
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
void ZHeterogeneousPopulation::zp_setHealthStateForId(quint64 id,
                                                      HealthStatus healthStatus,
                                                      bool* ok)
{
    ZPositionedIndividual* individual = zv_individualHash.value(id, nullptr);
    if (!individual)
    {
        if (ok)
        {
            *ok = false;
        }
        return;
    }

    HealthStatus status = static_cast<HealthStatus>(healthStatus);
    individual->zp_setHealthState(status);

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
    QHash<quint64, ZPositionedIndividual*>::const_iterator it;
    for (it = zv_individualHash.begin(); it != zv_individualHash.end(); ++it)
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
    ZPositionedIndividual* individual = zv_individualHash.value(id, nullptr);
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
    ZPositionedIndividual* individual = zv_individualHash.value(id, nullptr);
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
    if (index < 0 || index >= zv_individualHash.count())
    {
        if (ok)
        {
            *ok = false;
        }
        return;
    }

    id = zv_individualHash.keys().at(index);
    if (ok)
    {
        *ok = true;
    }
    return;
}
//============================================================
void ZHeterogeneousPopulation::zp_distansesForId(quint64 id,
                                                 QList<ZDistance>& distances,
                                                 bool* ok) const
{
    if (!zv_individualHash.keys().contains(id))
    {
        if (ok)
        {
            *ok = false;
        }
        return;
    }

    zv_distanceRepository->zp_distancesForId(id, distances);
    if (ok)
    {
        *ok = true;
    }
    return;
}
//============================================================
void ZHeterogeneousPopulation::zp_populationSize(quint64& size) const
{
    size = zv_individualHash.count();
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
    QHash<quint64, ZPositionedIndividual*>::const_iterator it;
    for (it = zv_individualHash.begin(); it != zv_individualHash.end(); ++it)
    {
        if (it.value()->zp_healthState() == healthStatus)
        {
            count++;
        }
    }

    return count;
}
//============================================================
void ZHeterogeneousPopulation::zp_notifyPopulationHealthStatus() const
{
    QMap<QString, quint64> populationHealthStatus;
    zp_populationHealthStatusReport(populationHealthStatus);
    emit zg_populationStateChanged(populationHealthStatus, true);
}
//============================================================
void ZHeterogeneousPopulation::zh_onPopulationStateChange() const
{
    QMap<QString, quint64> populationHealthStatus;
    zp_populationHealthStatusReport(populationHealthStatus);
    emit zg_populationStateChanged(populationHealthStatus);
}
//============================================================
void ZHeterogeneousPopulation::zp_individualHealthChanged(quint64 id, HealthStatus healthState) const
{
    emit zg_individualHealthChanged(id, healthState);
}
//============================================================
