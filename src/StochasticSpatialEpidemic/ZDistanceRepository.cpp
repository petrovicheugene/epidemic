//============================================================
#include "ZDistanceRepository.h"
#include <omp.h>
#include <QDebug>
#include <QtMath>
//============================================================
ZDistanceRepository::ZDistanceRepository(QObject* parent) : QObject(parent)
{
}
//============================================================
void ZDistanceRepository::zp_onPositionChange(quint64 id, QPointF pos)
{
    QMap<quint64, qreal> currentIdDistances;
    QMap<quint64, QMap<quint64, qreal>>::iterator it;
    qreal currentDistance;
    bool ok = false;
    QPointF currentPosition;

    for (it = zv_distanceMap.begin(); it != zv_distanceMap.end(); ++it)
    {
        quint64 currentId = it.key();
        if (currentId == id)
        {
            continue;
        }

        ok = false;
        currentPosition = QPointF();
        emit zg_inquirePositionForId(currentId, currentPosition, &ok);
        if (!ok)
        {
            continue;
        }

        qreal Xsq = pow((pos.x() - currentPosition.x()), 2.0);
        qreal Ysq = pow((pos.y() - currentPosition.y()), 2.0);
        currentDistance = qSqrt(Xsq + Ysq);

        currentIdDistances[currentId] = currentDistance;
        zv_distanceMap[currentId][id] = currentDistance;
    }

    zv_distanceMap[id] = currentIdDistances;
}
//============================================================
void ZDistanceRepository::zp_onPositionChange1(quint64 id, QPointF pos)
{
    QMap<quint64, qreal> currentIdDistances;
    // QMap<quint64, QMap<quint64, qreal>>::iterator it;
    qreal currentDistance;

    bool parallelAvailable = true; // !static_cast<bool>(omp_in_parallel());
#pragma omp parallel if (parallelAvailable)
    {
#pragma omp for
        // for (it = zv_distanceMap.begin(); it != zv_distanceMap.end(); ++it)
        for (int i = 0; i < zv_distanceMap.keys().count(); ++i)
        {
            quint64 currentId = zv_distanceMap.keys().at(i);
            if (currentId == id)
            {
                continue;
            }

            if (!zh_calcDistanceForId(currentId, pos, currentDistance))
            {
                continue;
            }

#pragma omp critical
            {
                currentIdDistances[currentId] = currentDistance;
                zv_distanceMap[currentId][id] = currentDistance;
            }
        }
    }

    zv_distanceMap[id] = currentIdDistances;
}
//============================================================
bool ZDistanceRepository::zh_calcDistanceForId(quint64 currentId,
                                               QPointF hostPos,
                                               qreal& distance) const
{
    bool ok = false;
    QPointF currentPosition;
    emit zg_inquirePositionForId(currentId, currentPosition, &ok);
    if (!ok)
    {
        return false;
    }

    qreal Xsq = pow((hostPos.x() - currentPosition.x()), 2.0);
    qreal Ysq = pow((hostPos.y() - currentPosition.y()), 2.0);
    distance = qSqrt(Xsq + Ysq);

    return true;
}

//============================================================
void ZDistanceRepository::zp_distancesForId(quint64 id, QMap<quint64, qreal>& distanceMap)
{
    distanceMap = zv_distanceMap.value(id);
}
//============================================================
