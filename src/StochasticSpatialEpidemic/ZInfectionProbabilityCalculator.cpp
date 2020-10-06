//============================================================
#include "ZInfectionProbabilityCalculator.h"
#include <QtMath>

//============================================================
ZInfectionProbabilityCalculator::ZInfectionProbabilityCalculator(QObject *parent) : QObject(parent)
{
    zv_L = 10;
}
//============================================================
qreal ZInfectionProbabilityCalculator::zp_calcProbability(QList<qreal> distances) const
{
    if (distances.isEmpty())
    {
        return 0.0;
    }

    qreal product = 1.0;
    for (int i = 0; i < distances.count(); ++i)
    {
        product *= 1.0 - (zv_infectionFactor * exp(-(distances.at(i) / zv_L)));
    }

    if (product < 0.0)
    {
        product = 0.0;
    }
    else if (product > 1.0)
    {
        product = 1.0;
    }
    return 1.0 - product;
}
//============================================================
void ZInfectionProbabilityCalculator::zp_setLParameter(qreal L)
{
    zv_L = L;
}
//============================================================
void ZInfectionProbabilityCalculator::zp_LParameter(qreal& L) const
{
    L = zv_L;
}
//============================================================
