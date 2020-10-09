//============================================================
#include "ZPositionedIndividual.h"
//============================================================
// STATIC
quint64 ZPositionedIndividual::nextId = 1;
// END STATIC
//============================================================
ZPositionedIndividual::ZPositionedIndividual(QObject* parent) : QObject(parent)
{
    zv_id = nextId++;
    zv_healthState = HS_NOT_DEFINED;
    zv_recoveryProbability = 0.0;
    zv_position = QPointF();
    zv_positionValidity = false;
}
//============================================================
ZPositionedIndividual::ZPositionedIndividual(QPointF position,
                                             HealthStatus healthState,
                                             QObject* parent)
    : QObject(parent)
{
    zv_id = nextId++;
    zv_healthState = healthState;
    zv_recoveryProbability = 0.0;
    zv_position = position;
    zv_positionValidity = true;
}
//============================================================
HealthStatus ZPositionedIndividual::zp_healthState() const
{
    return zv_healthState;
}
//============================================================
QPointF ZPositionedIndividual::zp_position() const
{
    return zv_position;
}
//============================================================
void ZPositionedIndividual::zp_setHealthState(HealthStatus healthState)
{
    if (zv_healthState == healthState)
    {
        return;
    }

    zv_healthState = healthState;
    zv_recoveryProbability = 0.0;
    emit zg_healthStateChanged(zv_id, zv_healthState);
}
//============================================================
void ZPositionedIndividual::zp_setPosition(QPointF position)
{
    if (zv_position == position && zv_positionValidity)
    {
        return;
    }

    zv_position = position;
    emit zg_positionChanged(zv_id, zv_position);
}
//============================================================
quint64 ZPositionedIndividual::zp_id() const
{
    return zv_id;
}
//============================================================
qreal ZPositionedIndividual::zp_recoveryProbability() const
{
    return zv_recoveryProbability;
}
//============================================================
void ZPositionedIndividual::zp_setRecoveryProbability(qreal probability)
{
    zv_recoveryProbability = probability;
}
//============================================================
void ZPositionedIndividual::zp_notifyPosition() const
{
    emit zg_positionChanged(zv_id, zv_position);
}
//============================================================
