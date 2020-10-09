//============================================================
#include "ZDistanceRepository.h"
#include <omp.h>
#include <QDebug>
#include <QElapsedTimer>
#include <QtMath>
//============================================================
ZDistanceRepository::ZDistanceRepository(QObject* parent) : QObject(parent)
{
}
//============================================================
void ZDistanceRepository::zp_onPositionChange(quint64 id, QPointF pos)
{
    QMap<quint64, QPointF> positionMap;
    zg_inquireIndividualPositionMap(positionMap);
    zh_recalcSortedDistanceMapForId(id, pos, positionMap);
}
//============================================================
void ZDistanceRepository::zp_recalcSortedDistanceMap()
{
    zp_clearDistances();
    QMap<quint64, QPointF> positionMap;
    zg_inquireIndividualPositionMap(positionMap);

    int posCount = positionMap.count();
    QList<quint64> keys = positionMap.keys();
    int t;
    int i;
    QPointF thePos;
    quint64 theId;
    QPointF aPos;
    quint64 aId;
    qreal aXsq;
    qreal aYsq;
    qreal aDistance;

#pragma omp parallel \
    shared(positionMap, \
           keys, \
           posCount) private(t, i, thePos, theId, aPos, aId, aXsq, aYsq, aDistance)
    {
#pragma omp for nowait
        for (t = 0; t < posCount; ++t)
        {
            theId = keys[t];
            thePos = positionMap[theId];

            for (i = t + 1; i < posCount; ++i)
            {
                aId = keys[i];
                aPos = positionMap[aId];

                aXsq = pow((thePos.x() - aPos.x()), 2.0);
                aYsq = pow((thePos.y() - aPos.y()), 2.0);
                aDistance = qSqrt(aXsq + aYsq);
#pragma omp critical
                {
                    zv_sortedDistanceMap[theId].insert(aDistance, aId);
                    zv_sortedDistanceMap[aId].insert(aDistance, theId);
                }
            }
        }
    }
}
//============================================================
void ZDistanceRepository::zh_recalcSortedDistanceMapForId(quint64 id,
                                                          QPointF thePos,
                                                          const QMap<quint64, QPointF>& positionMap)
{
    QMultiMap<qreal, quint64> distances;
    QMap<qreal, quint64>::iterator it;
    int i;
    qreal cDistance;
    quint64 cId;
    qreal cXsq;
    qreal cYsq;
    QPointF cPos;
    QList<quint64> keys = positionMap.keys();
    keys.removeAt(keys.indexOf(id));
    int posCount = keys.count();

#pragma omp parallel shared(id, thePos, distances, positionMap, keys, posCount) private(i, \
                                                                                        cDistance, \
                                                                                        cId, \
                                                                                        cXsq, \
                                                                                        cYsq, \
                                                                                        it)
    {
#pragma omp for
        for (i = 0; i < posCount; ++i)
        {
            cId = keys.at(i);
            cPos = positionMap[cId];
            cXsq = pow((thePos.x() - cPos.x()), 2.0);
            cYsq = pow((thePos.y() - cPos.y()), 2.0);
            cDistance = qSqrt(cXsq + cYsq);

#pragma omp critical
            {
                distances.insert(cDistance, cId);
            }

#pragma omp critical
            {
                it = zv_sortedDistanceMap[cId].find(zv_sortedDistanceMap[cId].key(id), id);
            }
            if (it != zv_sortedDistanceMap[cId].end())
            {
#pragma omp critical
                {
                    zv_sortedDistanceMap[cId].erase(it);
                }
            }

#pragma omp critical
            {
                zv_sortedDistanceMap[cId].insert(cDistance, id);
            }
        }
#pragma omp barrier
    }

    zv_sortedDistanceMap[id] = distances;
}
//============================================================
void ZDistanceRepository::zp_removeDistancesForId(quint64 id)
{
    zv_sortedDistanceMap.remove(id);

    int count = zv_sortedDistanceMap.count();
    int t;
    QMultiMap<qreal, quint64>::iterator it;

    for (t = 0; t < count; ++t)
    {
        it = zv_sortedDistanceMap[t].find(zv_sortedDistanceMap[t].key(id), id);
        if (it != zv_sortedDistanceMap[t].end())
        {
            zv_sortedDistanceMap[t].erase(it);
        }
    }
}
//============================================================
QMultiMap<qreal, quint64> ZDistanceRepository::zh_calcDistanceMapForId(
    quint64 id, QPointF thePos, const QMap<quint64, QPointF>& positionMap) const
{
    QMultiMap<qreal, quint64> distances;
    int i;
    qreal cDistance;
    quint64 cId;
    qreal cXsq;
    qreal cYsq;
    QPointF cPos;
    QList<quint64> keys = positionMap.keys();
    keys.removeAt(keys.indexOf(id));
    int posCount = keys.count();

#pragma omp parallel shared(distances, \
                            positionMap, \
                            keys, \
                            posCount) private(i, cDistance, cId, cXsq, cYsq)
    {
#pragma omp for
        for (i = 0; i < posCount; ++i)
        {
            cId = keys.at(i);
            cPos = positionMap[cId];
            cXsq = pow((thePos.x() - cPos.x()), 2.0);
            cYsq = pow((thePos.y() - cPos.y()), 2.0);
            cDistance = qSqrt(cXsq + cYsq);

#pragma omp critical
            {
                distances.insert(cDistance, cId);
            }
        }
#pragma omp barrier
    }

    return distances;
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
void ZDistanceRepository::zp_sortedDistancesForId(quint64 id,
                                                  QMultiMap<qreal, quint64>& sortedDistanceMap)
{
    sortedDistanceMap = zv_sortedDistanceMap.value(id);
}
//============================================================
void ZDistanceRepository::zp_clearDistances()
{
    zv_sortedDistanceMap.clear();
}
//============================================================
