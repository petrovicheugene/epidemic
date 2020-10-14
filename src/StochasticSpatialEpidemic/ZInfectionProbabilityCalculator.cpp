//============================================================
#include "ZInfectionProbabilityCalculator.h"

#include <omp.h>
#include <QDebug>
#include <QtMath>
//============================================================
ZInfectionProbabilityCalculator::ZInfectionProbabilityCalculator(QObject* parent) : QObject(parent)
{
    zp_setLParameter(5);
}
//============================================================
qreal ZInfectionProbabilityCalculator::zp_calcProbability(QList<qreal> distances)
{
    if (distances.isEmpty())
    {
        return 0.0;
    }
    qreal commonProduct = 1.0;
    qreal product = 1.0;
    int i;
    qreal saftyDistance;
    qreal stayHealthyProbability;

    //#pragma omp parallel shared(distances, commonProduct) private(i, \
    //                                                              product, \
    //                                                              saftyDistance, \
    //                                                              stayHealthyProbability)
    {
        product = 1.0;
        commonProduct = 1.0;
        saftyDistance = zv_safetyDistance;
        stayHealthyProbability = 0;
        //#pragma omp for
        for (i = 0; i < distances.count(); ++i)
        {
            stayHealthyProbability = 1.0 - (zv_infectionFactor * exp(-(distances.at(i) / zv_L)));
            product *= stayHealthyProbability;
        }

        if (product < 0.0)
        {
            product = 0.0;
        }
        else if (product > 1.0)
        {
            product = 1.0;
        }
        //#pragma omp critical
        {
            commonProduct *= product;
        }

        //#pragma omp critical
        {
            zv_safetyDistance = qMin(zv_safetyDistance, saftyDistance);
        }

        //#pragma omp barrier
    }

    return 1.0 - commonProduct;
}
//============================================================
qreal ZInfectionProbabilityCalculator::zp_safetyDistance() const
{
    return zv_safetyDistance;
}
//============================================================
void ZInfectionProbabilityCalculator::zp_setLParameter(qreal L)
{
    zv_L = L;
    recalcSafetyDistance();
}
//============================================================
void ZInfectionProbabilityCalculator::zp_LParameter(qreal& L) const
{
    L = zv_L;
}
//============================================================
void ZInfectionProbabilityCalculator::recalcSafetyDistance()
{
    zv_safetyDistance = static_cast<qreal>(__INT64_MAX__);
    qreal min = 0;
    qreal mid;
    qreal max = 200;
    qreal pMid;
    qreal infProbability = 0.01;
    qreal precision = 0.01;

    for (int canary = 0; canary < 500; ++canary)
    {
        mid = min + (max - min) / 2;
        pMid = (zv_infectionFactor * exp(-(mid / zv_L)));

        if (pMid <= infProbability)
        {
            max = mid;
        }
        else
        {
            min = mid;
        }

        if ((max - min) < precision)
        {
            zv_safetyDistance = min;
            return;
        }
    }

    // zv_safetyDistance = static_cast<qreal>(__INT64_MAX__);
}
//============================================================
