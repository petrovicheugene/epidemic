//============================================================
#ifndef ZHETEROGENEOUSPOPULATION_H
#define ZHETEROGENEOUSPOPULATION_H
//============================================================
#include "ZAbstractPopulation.h"
#include "ZDistance.h"
#include "ZGenerationSettings.h"
#include "ZStochasticHeterogeneousProcessCommon.h"

#include <QHash>
#include <QMap>
#include <QPointF>
//============================================================
class ZDistanceRepository;
class ZPositionedIndividual;
//============================================================
class ZHeterogeneousPopulation : public ZAbstractPopulation
{
    Q_OBJECT
public:
    explicit ZHeterogeneousPopulation(QObject* parent = nullptr);

public slots:

    void zp_generate(QVariant vSettings) override;
    void zp_clear() override;

    void zp_setHealthStatus(HealthStatus heathStatus);

    void zp_individualPosition(quint64 id, QPointF& position, bool* ok = nullptr) const;
    void zp_individualPositionHash(QHash<quint64, QPointF>& positionHash) const;
    void zp_healthStateForId(quint64 id, HealthStatus& healthState, bool* ok = nullptr) const;
    void zp_setHealthStateForId(quint64 id, HealthStatus healthState, bool* ok = nullptr);
    void zp_recoveryProbabilityForId(quint64 id, qreal& probability, bool* ok = nullptr);
    void zp_setRecoveryProbabilityForId(quint64 id, qreal probability, bool* ok = nullptr);
    void zp_idListForHealthState(HealthStatus healthState, QList<quint64>& idList) const;

    void zp_idForIndex(int index, quint64& id, bool* ok = nullptr) const;
    void zp_distansesForId(quint64 id, QList<ZDistance>& distances, bool* ok = nullptr) const;
    void zp_populationSize(quint64& size) const override;
    void zp_populationHealthStatus(QMap<int, quint64>& populationHealthStatus) const override;
    void zp_populationHealthStatusReport(
        QMap<QString, quint64>& populationHealthStatus) const override;

    quint64 zp_numberForHealthStatus(int healthStatus) const override;
    void zp_notifyPopulationHealthStatus() const override;

signals:

    void zg_individualAdded(quint64 id) const;
    void zg_individualListAdded(QList<std ::tuple<quint64, QPointF, HealthStatus>>) const;
    void zg_individualRemoved(quint64 id) const;
    void zg_allIindividualsRemoved() const;
    void zg_individualPositionChanged(quint64 id, QPointF position) const;
    void zg_individualHealthChanged(quint64 id, HealthStatus healthState) const;

private slots:

    void zh_onPopulationStateChange() const;
    void zp_individualHealthChanged(quint64 id, HealthStatus healthState) const;

private:
    // VARS
    ZDistanceRepository* zv_distanceRepository;
    //QMap<quint64, ZPositionedIndividual*> zv_individuals;
    QHash<quint64, ZPositionedIndividual*> zv_individualHash;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
};
//============================================================
#endif // ZHETEROGENEOUSPOPULATION_H
