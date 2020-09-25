//============================================================
#ifndef ZINDIVIDUAL_H
#define ZINDIVIDUAL_H
//============================================================
#include <QObject>
#include <QPointF>
//============================================================
class ZPositionedIndividual : public QObject
{
    Q_OBJECT
public:
    enum HealthState
    {
        HS_NOT_DEFINED = 0,
        HS_SUSCEPTIBLE = 1,
        HS_INFECTIOUS = 2,
        HS_RECOVERED = 3
    };

    explicit ZPositionedIndividual(QObject* parent = nullptr);
    explicit ZPositionedIndividual(QPointF position,
                                   HealthState healthState,
                                   QObject* parent = nullptr);

    ZPositionedIndividual::HealthState zp_healthState() const;
    void zp_setHealthState(ZPositionedIndividual::HealthState healthState);

    QPointF zp_position() const;
    void zp_setPosition(QPointF position);
    quint64 zp_id() const;

signals:

    void zg_positionChanged(quint64 id, QPointF position) const;
    void zg_healthStateChanged(quint64 id, ZPositionedIndividual::HealthState healthState) const;

public slots:

private:
    //ZPositionedIndividualealthState zv_healthState;
    static quint64 nextId;
    quint64 zv_id;
    QPointF zv_position;
    HealthState zv_healthState;
    // FUNCS
};
//============================================================
#endif // ZINDIVIDUAL_H
