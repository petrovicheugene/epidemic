//============================================================
#include "ZRecoveryProbabilityCalculator.h"
#include <QDebug>
//============================================================
ZRecoveryProbabilityCalculator::ZRecoveryProbabilityCalculator(QObject *parent) : QObject(parent)
{
    zv_startingProbability = 0.02;
    zv_recoveryDurationFactor = 0.3;
}
//============================================================
qreal ZRecoveryProbabilityCalculator::zp_calcProbability(qreal probability)
{
    qreal newProbability;
    if (probability == 0)
    {
        newProbability = zv_startingProbability;
    }
    else
    {
        if (zv_cacheMap.contains(probability))
        {
            return zv_cacheMap.value(probability);
        }

        // logistic function
        newProbability = zv_recoveryDurationFactor * probability * (1 - probability) + probability;
    }

    if (probability > 1.0)
    {
        newProbability = 1.0;
    }
    else if (probability < 0.0)
    {
        newProbability = 0.0;
    }

    zv_cacheMap.insert(probability, newProbability);
    return newProbability;
}
//============================================================
void ZRecoveryProbabilityCalculator::zp_setStartingProbability(qreal probability)
{
    zv_cacheMap.clear();

    if (probability < 0.0)
    {
        zv_startingProbability = 0.0;
    }
    else if (probability > 1.0)
    {
        zv_startingProbability = 1.0;
    }
    else
    {
        zv_startingProbability = probability;
    }
}
//============================================================
void ZRecoveryProbabilityCalculator::zp_startingProbability(qreal& probability) const
{
    probability = zv_startingProbability;
}
//============================================================
void ZRecoveryProbabilityCalculator::zp_setRecoveryDurationFactor(qreal factor)
{
    zv_cacheMap.clear();
    zv_recoveryDurationFactor = factor;
}
//============================================================
void ZRecoveryProbabilityCalculator::zp_recoveryDurationFactor(qreal& factor) const
{
    factor = zv_recoveryDurationFactor;
}
//============================================================
