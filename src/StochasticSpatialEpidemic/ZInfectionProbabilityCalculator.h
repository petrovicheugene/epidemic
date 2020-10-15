//============================================================
#ifndef ZINFECTIONPROBABILITYCALCULATOR_H
#define ZINFECTIONPROBABILITYCALCULATOR_H
//============================================================
#include <QCache>
#include <QObject>
//============================================================
class ZInfectionProbabilityCalculator : public QObject
{
    Q_OBJECT
public:
    explicit ZInfectionProbabilityCalculator(QObject* parent = nullptr);

    qreal zp_calcProbability(QList<qreal> distances);
    qreal zp_safetyDistance() const;
public slots:
    void zp_setLParameter(qreal L);
    void zp_LParameter(qreal& L) const;
signals:

private:
    // VARS
    QCache<qreal, qreal> zv_cache;
    const qreal zv_infectionFactor = 1.36;
    qreal zv_L;
    qreal zv_safetyDistance;

    // FUNCS
    void recalcSafetyDistance();

    // infection factor -
    // by one probability of infection is 0.5 per day in the distance L
};
//============================================================
#endif // ZINFECTIONPROBABILITYCALCULATOR_H
