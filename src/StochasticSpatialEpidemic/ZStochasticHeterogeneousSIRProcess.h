//============================================================
#ifndef ZSIREPIDEMICPROCESS_H
#define ZSIREPIDEMICPROCESS_H
//============================================================
#include "ZAbstractEpidemicProcess.h"
#include "ZDistance.h"
#include "ZStochasticHeterogeneousProcessCommon.h"
//============================================================
class QTimer;

class ZInfectionProbabilityCalculator;
class ZRecoveryProbabilityCalculator;
//============================================================
class ZStochasticHeterogeneousSIRProcess : public ZAbstractEpidemicProcess
{
    Q_OBJECT
public:
    explicit ZStochasticHeterogeneousSIRProcess(QObject* parent = nullptr);

    void zp_setPopulation(ZAbstractPopulation* population) override;

public slots:

    void zp_dispatchCommand(int command, QVariant data = QVariant()) override;
    void zp_onPopulationChange();
    void zp_processStep(int& step) const override;

signals:

    void zg_inquireHealthStatusForId(quint64 id,
                                     HealthStatus& healthState,
                                     bool* ok = nullptr) const;
    void zg_invokeSetHealthStatusForId(quint64 id,
                                       HealthStatus healthState,
                                       bool* ok = nullptr) const;
    void zg_inquireDistancesForId(quint64 id, QList<ZDistance>& distances, bool* ok = nullptr) const;
    void zg_inquireIdForIndex(int index, quint64& id, bool* ok = nullptr) const;
    void zg_inquirePopulationSize(quint64& count) const;
    void zg_inquireIdListForHealthState(HealthStatus healthState, QList<quint64>& idList) const;
    void zg_inquireRecoveryProbabilityForId(quint64 id,
                                            qreal& recoveryProbability,
                                            bool* ok = nullptr) const;
    void zg_invokeSetRecoveryProbabilityForId(quint64 id,
                                              qreal recoveryProbability,
                                              bool* ok = nullptr) const;
    void zg_invokeSetHealthStatus(HealthStatus healthStatus) const;
    // void zg_groupSizeChanged(const QString& groupName, int size) const;
    void zg_processChanged(int status, QVariant data = QVariant()) const;

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    // VARS
    ZInfectionProbabilityCalculator* zv_infectionProbabilityCalculator;
    ZRecoveryProbabilityCalculator* zv_recoveryProbabilityCalculator;
    int zv_epidemicDay;
    QTimer* zv_timer;
    ProcessStatus zv_processStatus;
    QMap<HealthStatus, QList<quint64>> zv_initPopulationHealthStateMap;
    bool zv_needRestoringInitState;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_restoreInitPopulationState();
    void zh_saveInitPopulationState();
    void zh_runEpidemic();
    void zh_makeEpidemicStep();

    void zh_cureInfectious(QList<quint64>& infectiousList) const;
    void zh_infectSusceptible(QList<quint64>& infectiousList, QList<quint64>& susceptibleList) const;
    bool zh_infectOneSusceptible(quint64 id, QList<quint64>& infectiousList) const;

    void zh_pauseEpidemic();

    void zh_finishEpidemic();
    void zh_setTimerInterval(QVariant data);
    void zh_setTimerInterval(int interval);

    void zh_setProcessStatus(ProcessStatus status);
    void zh_setLParameter(QVariant data);
    void zh_setRecoveryDurationFactor(QVariant data);
    void zh_setRecoveryStartingProbability(QVariant data);
    void zh_resetPopulationHealthStatus();
};
//============================================================
#endif // ZSIREPIDEMICPROCESS_H
