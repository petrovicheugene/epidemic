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
    zv_position = QPointF(0, 0);
}
//============================================================
ZPositionedIndividual::ZPositionedIndividual(QPointF position,
                                             HealthState healthState,
                                             QObject* parent)
    : QObject(parent)
{
    zv_id = nextId++;
    zv_healthState = healthState;
    zv_position = position;
}
//============================================================
ZPositionedIndividual::HealthState ZPositionedIndividual::zp_healthState() const
{
    return zv_healthState;
}
//============================================================
QPointF ZPositionedIndividual::zp_position() const
{
    return zv_position;
}
//============================================================
void ZPositionedIndividual::zp_setHealthState(ZPositionedIndividual::HealthState healthState)
{
    if (zv_healthState == healthState)
    {
        return;
    }

    zv_healthState = healthState;
    emit zg_healthStateChanged(zv_id, zv_healthState);
}
//============================================================
void ZPositionedIndividual::zp_setPosition(QPointF position)
{
    if (zv_position == position)
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
