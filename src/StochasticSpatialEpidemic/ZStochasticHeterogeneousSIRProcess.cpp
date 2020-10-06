//============================================================
#include "ZStochasticHeterogeneousSIRProcess.h"
#include "ZHeterogeneousPopulation.h"
#include "ZInfectionProbabilityCalculator.h"
#include "ZRecoveryProbabilityCalculator.h"

#include <QDebug>
#include <QRandomGenerator>
#include <QTimer>
//============================================================
ZStochasticHeterogeneousSIRProcess::ZStochasticHeterogeneousSIRProcess(QObject* parent)
    : ZAbstractEpidemicProcess(parent)
{
    zv_processStatus = PS_STOPPED;
    zv_needRestoringInitState = false;
    zh_createComponents();
    zh_createConnections();
    zv_epidemicDay = 0;
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_createComponents()
{
    zv_timer = new QTimer(this);
    zv_infectionProbabilityCalculator = new ZInfectionProbabilityCalculator(this);
    zv_recoveryProbabilityCalculator = new ZRecoveryProbabilityCalculator(this);
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_createConnections()
{
    connect(zv_timer,
            &QTimer::timeout,
            this,
            &ZStochasticHeterogeneousSIRProcess::zh_makeEpidemicStep);
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zp_setPopulation(ZAbstractPopulation* abstractPopulation)
{
    ZHeterogeneousPopulation* population = qobject_cast<ZHeterogeneousPopulation*>(
        abstractPopulation);

    if (!population)
    {
        return;
    }

    connect(population,
            &ZHeterogeneousPopulation::zg_populationStateChanged,
            this,
            &ZStochasticHeterogeneousSIRProcess::zp_onPopulationChange);

    connect(this,
            &ZStochasticHeterogeneousSIRProcess::zg_inquirePopulationSize,
            population,
            &ZHeterogeneousPopulation::zp_populationSize);
    connect(this,
            &ZStochasticHeterogeneousSIRProcess::zg_inquireIdForIndex,
            population,
            &ZHeterogeneousPopulation::zp_idForIndex);
    connect(this,
            &ZStochasticHeterogeneousSIRProcess::zg_inquireDistancesForId,
            population,
            &ZHeterogeneousPopulation::zp_distansesForId);
    connect(this,
            &ZStochasticHeterogeneousSIRProcess::zg_inquireHealthStatusForId,
            population,
            &ZHeterogeneousPopulation::zp_healthStateForId);
    connect(this,
            &ZStochasticHeterogeneousSIRProcess::zg_invokeSetHealthStatusForId,
            population,
            &ZHeterogeneousPopulation::zp_setHealthStateForId);

    connect(this,
            &ZStochasticHeterogeneousSIRProcess::zg_inquireIdListForHealthState,
            population,
            &ZHeterogeneousPopulation::zp_idListForHealthState);
    connect(this,
            &ZStochasticHeterogeneousSIRProcess::zg_inquireRecoveryProbabilityForId,
            population,
            &ZHeterogeneousPopulation::zp_recoveryProbabilityForId);
    connect(this,
            &ZStochasticHeterogeneousSIRProcess::zg_invokeSetRecoveryProbabilityForId,
            population,
            &ZHeterogeneousPopulation::zp_setRecoveryProbabilityForId);
    connect(this,
            &ZStochasticHeterogeneousSIRProcess::zg_invokeSetHealthStatus,
            population,
            &ZHeterogeneousPopulation::zp_setHealthStatus);
}
//============================================================
QStringList ZStochasticHeterogeneousSIRProcess::zp_groupList() const
{
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zp_onPopulationChange()
{
    if (zv_processStatus == PS_STOPPED)
    {
        zv_needRestoringInitState = false;
    }
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zp_processStep(int& step) const
{
    step = zv_epidemicDay;
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zp_dispatchCommand(int command, QVariant data)
{
    EpidemicCommand epidemicCommand = static_cast<EpidemicCommand>(command);
    qDebug() << "COMMAND";
    switch (epidemicCommand)
    {
    case EC_STOP:
        zh_finishEpidemic();
        break;
    case EC_RUN:
        zh_runEpidemic();
        break;
    case EC_PAUSE:
        zh_pauseEpidemic();
        break;
    case EC_SET_TIMER_INTERVAL:
        zh_setTimerInterval(data);
        break;
    case EC_RECALC_RECOVERY_PROBABILITY:
        zh_recalcRecoveryProbability(data);
        break;
    case EC_SET_L_PARAMETER:
        zh_setLParameter(data);
        break;
    case EC_RESET_HEALTH_STATUS:
        zh_resetPopulationHealthStatus();
        break;
    }
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_runEpidemic()
{
    if (zv_processStatus == PS_STOPPED)
    {
        if (zv_needRestoringInitState)
        {
            zh_restoreInitPopulationState();
        }
        else
        {
            zh_saveInitPopulationState();
        }
    }
    else if (zv_processStatus == PS_SUSPENDED)
    {
    }

    zh_setProcessStatus(PS_RUNNING);
    zv_timer->start();
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_makeEpidemicStep()
{
    // cure the infectious
    ++zv_epidemicDay;
    QList<quint64> infectiousList;
    emit zg_inquireIdListForHealthState(HS_INFECTIOUS, infectiousList);
    zh_cureInfectious(infectiousList);

    if (infectiousList.isEmpty())
    {
        zh_finishEpidemic();
    }

    QList<quint64> susceptibleList;
    emit zg_inquireIdListForHealthState(HS_SUSCEPTIBLE, susceptibleList);
    zh_infectSusceptible(infectiousList, susceptibleList);
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_cureInfectious(QList<quint64>& infectiousList) const
{
    qreal probability = 0.0;
    bool ok = false;

    for (int i = infectiousList.count() - 1; i >= 0; --i)
    {
        quint64 id = infectiousList.at(i);
        emit zg_inquireRecoveryProbabilityForId(id, probability, &ok);
        if (!ok)
        {
            continue;
        }

        probability = zv_recoveryProbabilityCalculator->zp_calcProbability(probability);
        // throw a dice
        if (QRandomGenerator::global()->generateDouble() < probability)
        {
            emit zg_invokeSetHealthStatusForId(id, HS_RECOVERED);
            infectiousList.removeAt(i);
        }
        else
        {
            emit zg_invokeSetRecoveryProbabilityForId(id, probability);
        }
    }
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_infectSusceptible(QList<quint64>& infectiousList,
                                                              QList<quint64>& susceptibleList) const
{
    // TODO Check the func (product equals 1 every time )
    quint64 id;
    bool ok;

    qreal probability = 0.0;
    QList<quint64> newInfectedList;
    for (int i = susceptibleList.count() - 1; i >= 0; --i)
    {
        id = susceptibleList.at(i);
        QMap<quint64, qreal> distances;
        emit zg_inquireDistancesForId(id, distances, &ok);

        if (!ok)
        {
            continue;
        }

        QMap<quint64, qreal>::iterator it;
        QList<qreal> infectiousdistances;
        for (it = distances.begin(); it != distances.end(); ++it)
        {
            if (!infectiousList.contains(it.key()))
            {
                continue;
            }

            infectiousdistances.append(it.value());
        }

        probability = zv_infectionProbabilityCalculator->zp_calcProbability(infectiousdistances);

        //                          product *= 1.0 - (zv_infectionFactor * exp(-(it.value() / zv_L)));
        // throw a dice
        // 1-product - probability of infection
        if (QRandomGenerator::global()->generateDouble() < probability)
        {
            emit zg_invokeSetHealthStatusForId(id, HS_INFECTIOUS);
            susceptibleList.removeAt(i);
            newInfectedList.append(id);
        }
    }

    infectiousList.append(newInfectedList);
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_pauseEpidemic()
{
    if (zv_processStatus == PS_RUNNING)
    {
        zv_timer->stop();
        zh_setProcessStatus(PS_SUSPENDED);
    }
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_finishEpidemic()
{
    if (zv_processStatus != PS_STOPPED)
    {
        zv_timer->stop();
        zv_epidemicDay = 0;
        zh_setProcessStatus(PS_STOPPED);
        emit zg_epidemicFinished();
    }
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_restoreInitPopulationState()
{
    QMap<HealthStatus, QList<quint64>>::const_iterator it;
    for (it = zv_initPopulationHealthStateMap.begin(); it != zv_initPopulationHealthStateMap.end();
         it++)
    {
        foreach (auto id, it.value())
        {
            emit zg_invokeSetHealthStatusForId(id, it.key());
        }
    }

    zv_needRestoringInitState = true;
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_saveInitPopulationState()
{
    zv_initPopulationHealthStateMap.clear();

    QList<quint64> idList;
    emit zg_inquireIdListForHealthState(HS_SUSCEPTIBLE, idList);
    zv_initPopulationHealthStateMap.insert(HS_SUSCEPTIBLE, idList);
    emit zg_inquireIdListForHealthState(HS_INFECTIOUS, idList);
    zv_initPopulationHealthStateMap.insert(HS_INFECTIOUS, idList);
    emit zg_inquireIdListForHealthState(HS_RECOVERED, idList);
    zv_initPopulationHealthStateMap.insert(HS_RECOVERED, idList);

    zv_needRestoringInitState = true;
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_setProcessStatus(ProcessStatus status)
{
    zv_processStatus = status;
    emit zg_processChanged(zv_processStatus);
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_recalcRecoveryProbability(QVariant data)
{
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_setLParameter(QVariant data)
{
    if (!data.isValid() || !data.canConvert<qreal>())
    {
        return;
    }

    bool ok = false;
    int L = data.toDouble(&ok);
    if (ok)
    {
        zv_infectionProbabilityCalculator->zp_setLParameter(L);
    }
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_resetPopulationHealthStatus()
{
    emit zg_invokeSetHealthStatus(HS_SUSCEPTIBLE);
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_setTimerInterval(QVariant data)
{
    if (!data.isValid() || !data.canConvert<int>())
    {
        return;
    }

    bool ok = false;
    int interval = data.toInt(&ok);
    if (ok)
    {
        zh_setTimerInterval(interval);
    }
}
//============================================================
void ZStochasticHeterogeneousSIRProcess::zh_setTimerInterval(int interval)
{
    zv_timer->setInterval(interval);
}
//============================================================
