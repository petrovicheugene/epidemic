//============================================================
#include "ZRecoveryProbabilityCalculator.h"
//============================================================
ZRecoveryProbabilityCalculator::ZRecoveryProbabilityCalculator(QObject *parent) : QObject(parent)
{
    zv_startingProbability = 0.02;
    zv_recoveryDurationFactor = 0.3;
}
//============================================================
qreal ZRecoveryProbabilityCalculator::zp_calcProbability(qreal probability) const
{
    if (probability == 0)
    {
        probability = zv_startingProbability;
    }
    else
    {
        // logistic function
        probability = zv_recoveryDurationFactor * probability * (1 - probability) + probability;
    }

    if (probability > 1.0)
    {
        probability = 1.0;
    }
    else if (probability < 0.0)
    {
        probability = 0.0;
    }

    return probability;
}
//============================================================
void ZRecoveryProbabilityCalculator::zp_setStartingProbability(qreal probability)
{
    if (probability < 0.0)
    {
        zv_startingProbability = 0.0;
    }
    else if (probability > 1.0)
    {
        zv_startingProbability = 1.0;
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
    zv_recoveryDurationFactor = factor;
}
//============================================================
void ZRecoveryProbabilityCalculator::zp_recoveryDurationFactor(qreal& factor) const
{
    factor = zv_recoveryDurationFactor;
}
//============================================================
