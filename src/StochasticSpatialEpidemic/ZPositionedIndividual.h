//============================================================
#ifndef ZINDIVIDUAL_H
#define ZINDIVIDUAL_H
//============================================================
#include <ZStochasticHeterogeneousProcessCommon.h>
#include <QObject>
#include <QPointF>
//============================================================
class ZPositionedIndividual : public QObject
{
    Q_OBJECT
public:
    explicit ZPositionedIndividual(QObject* parent = nullptr);
    explicit ZPositionedIndividual(QPointF position,
                                   HealthStatus healthState,
                                   QObject* parent = nullptr);

    HealthStatus zp_healthState() const;
    void zp_setHealthState(HealthStatus healthState);

    QPointF zp_position() const;
    void zp_setPosition(QPointF position);
    quint64 zp_id() const;
    qreal zp_recoveryProbability() const;
    void zp_setRecoveryProbability(qreal probability);

signals:

    void zg_positionChanged(quint64 id, QPointF position) const;
    void zg_healthStateChanged(quint64 id, HealthStatus healthState) const;

public slots:

private:
    //ZPositionedIndividualealthState zv_healthState;
    static quint64 nextId;

    quint64 zv_id;
    QPointF zv_position;
    bool zv_positionValidity;
    HealthStatus zv_healthState;
    qreal zv_recoveryProbability;

    // FUNCS
};
//============================================================
#endif // ZINDIVIDUAL_H
