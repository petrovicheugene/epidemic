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
void ZDistanceRepository::zp_recalcDistancesForPositions(const QHash<quint64, QPointF>& positionHash)
{
    QHash<quint64, QPointF> existingPositionHash;
    emit zg_inquireIndividualPositionHash(existingPositionHash);

    // replace positions in existingPositionHash with positions from the positionHash
    // in case the positions from the existingPositionHash are obsolete
    QHash<quint64, QPointF>::const_iterator it;
    for (it = positionHash.begin(); it != positionHash.end(); ++it)
    {
        existingPositionHash[it.key()] = it.value();
    }

    int n;
    int nCount = positionHash.keys().count();
    int e;
    int eCount = existingPositionHash.keys().count();
    QList<quint64> nKeys = positionHash.keys();
    QList<quint64> eKeys = existingPositionHash.keys();
    quint64 nId;
    quint64 eId;
    QPointF nPos;
    QPointF ePos;
    QSet<quint64> recalcedSet;
    qreal aXsq;
    qreal aYsq;
    qreal aDistance;
    int i;
    int distanceCount;
    bool ok;

    //#pragma omp parallel shared(eKeys, nKeys, positionHash, existingPositionHash, recalcedSet) private( \
//    n, e, i, nId, nPos, eId, ePos, aXsq, aYsq, aDistance, ok, distanceCount)
    {
        //#pragma omp for nowait
        for (n = 0; n < nCount; ++n)
        {
            nId = nKeys.at(n);
            nPos = positionHash.value(nId);
            //#pragma omp critical
            {
                recalcedSet.insert(nId);
            }
            for (e = 0; e < eCount; ++e)
            {
                eId = eKeys.at(e);
                if (recalcedSet.contains(eId))
                {
                    continue;
                }

                ePos = existingPositionHash.value(eId);

                aXsq = pow((nPos.x() - ePos.x()), 2.0);
                aYsq = pow((nPos.y() - ePos.y()), 2.0);
                aDistance = qSqrt(aXsq + aYsq);

                // update distanceHash
                ok = false;
                distanceCount = zv_distanceHash[nId].count();

                //#pragma omp critical
                {
                    for (i = 0; i < distanceCount; ++i)
                    {
                        if (zv_distanceHash[nId].at(i).zv_id != eId)
                        {
                            continue;
                        }
                        else
                        {
                            // found
                            zv_distanceHash[nId][i].zv_distance = aDistance;
                            ok = true;
                            break;
                        }
                    }
                    // not found. create new
                    if (!ok)
                    {
                        zv_distanceHash[nId].append(ZDistance(eId, aDistance));
                    }

                    distanceCount = zv_distanceHash[eId].count();
                    for (i = 0; i < distanceCount; ++i)
                    {
                        if (zv_distanceHash[eId].at(i).zv_id != nId)
                        {
                            continue;
                        }
                        else
                        {
                            // found
                            zv_distanceHash[eId][i].zv_distance = aDistance;
                            ok = true;
                            break;
                        }
                    }

                    // not found. create new
                    if (!ok)
                    {
                        zv_distanceHash[eId].append(ZDistance(nId, aDistance));
                    }
                }
            }

            //#pragma omp critical
            //            {
            //                commonRecalcedSet = commonRecalcedSet.unite(recalcedSet);
            //            }
            //#pragma omp barrier
            //            recalcedSet = commonRecalcedSet;
        }
    }

    // sorting
    QList<quint64> keys = zv_distanceHash.keys();

    for (int i = 0; i < zv_distanceHash.count(); ++i)
    {
        std::sort(zv_distanceHash[keys.at(i)].begin(), zv_distanceHash[keys.at(i)].end());
    }
}
//============================================================
QHash<quint64, QHash<quint64, qreal>> ZDistanceRepository::zh_distancesSortedById() const
{
    QHash<quint64, QHash<quint64, qreal>> idDistances;
    QHash<quint64, QList<ZDistance>>::const_iterator it;
    QHash<quint64, qreal> cHash;
    ZDistance distance;

    for (it = zv_distanceHash.begin(); it != zv_distanceHash.end(); ++it)
    {
        cHash.clear();
        for (int i = 0; i < it.value().count(); ++i)
        {
            distance = it.value().at(i);
            cHash.insert(distance.zv_id, distance.zv_distance);
        }

        idDistances[it.key()] = cHash;
    }

    return idDistances;
}
//============================================================
void ZDistanceRepository::zh_convertToDistanceHash(
    const QHash<quint64, QHash<quint64, qreal>>& distances)
{
    QHash<quint64, QList<ZDistance>> distanceCache; // total cache
    QList<ZDistance> distanceList;                  // new row

    QHash<quint64, QHash<quint64, qreal>>::const_iterator it; // total it
    QHash<quint64, qreal>::const_iterator rowIt;              // row it

    for (it = distances.begin(); it != distances.end(); ++it)
    {
        distanceList.clear();
        for (rowIt = it.value().begin(); rowIt != it.value().end(); ++it)
        {
            distanceList.append(ZDistance(rowIt.key(), rowIt.value()));
        }
        std::sort(distanceList.begin(), distanceList.end());
        distanceCache[it.key()] = distanceList;
    }

    zv_distanceHash = distanceCache;
}
//============================================================
void ZDistanceRepository::zp_recalcDistancesForPosition(quint64 id, QPointF pos)
{
    QList<ZDistance> distancesToPos;
    //    zh_calcDistancesForPos(pos, distancesToPos);
    //    zv_positionMap.insert(id, pos);
    //qDebug() << distancesToPos.count();
    //    for ()
    //    {
    //    }
}
//============================================================
//void ZDistanceRepository::zh_calcDistancesForPos(const QPointF& pos,
//                                                 QList<ZDistance>& distancesToPos) const
//{
//}
//============================================================
//void ZDistanceRepository::zp_onPositionChange1(quint64 id, QPointF pos)
//{
//    QMap<quint64, QPointF> positionMap;
//    zg_inquireIndividualPositionMap(positionMap);
//    zh_recalcSortedDistanceMapForId(id, pos, positionMap);
//}
//============================================================
//void ZDistanceRepository::zp_recalcSortedDistanceMap()
//{
//    zp_clearDistances();
//    QMap<quint64, QPointF> positionMap;
//    zg_inquireIndividualPositionMap(positionMap);

//    int posCount = positionMap.count();
//    QList<quint64> keys = positionMap.keys();
//    int t;
//    int i;
//    QPointF thePos;
//    quint64 theId;
//    QPointF aPos;
//    quint64 aId;
//    qreal aXsq;
//    qreal aYsq;
//    qreal aDistance;

//#pragma omp parallel \
//    shared(positionMap, \
//           keys, \
//           posCount) private(t, i, thePos, theId, aPos, aId, aXsq, aYsq, aDistance)
//    {
//#pragma omp for nowait
//        for (t = 0; t < posCount; ++t)
//        {
//            theId = keys[t];
//            thePos = positionMap[theId];

//            for (i = t + 1; i < posCount; ++i)
//            {
//                aId = keys[i];
//                aPos = positionMap[aId];

//                aXsq = pow((thePos.x() - aPos.x()), 2.0);
//                aYsq = pow((thePos.y() - aPos.y()), 2.0);
//                aDistance = qSqrt(aXsq + aYsq);
//#pragma omp critical
//                {
//                    zv_sortedDistanceMap[theId].insert(aDistance, aId);
//                    zv_sortedDistanceMap[aId].insert(aDistance, theId);
//                }
//            }
//        }
//    }
//}
//============================================================
//void ZDistanceRepository::zh_recalcSortedDistanceMapForId(quint64 id,
//                                                          QPointF thePos,
//                                                          const QMap<quint64, QPointF>& positionMap)
//{
//    QMultiMap<qreal, quint64> distances;
//    QMap<qreal, quint64>::iterator it;
//    int i;
//    qreal cDistance;
//    quint64 cId;
//    qreal cXsq;
//    qreal cYsq;
//    QPointF cPos;
//    QList<quint64> keys = positionMap.keys();
//    keys.removeAt(keys.indexOf(id));
//    int posCount = keys.count();

//#pragma omp parallel shared(id, thePos, distances, positionMap, keys, posCount) private(i, \
//                                                                                        cDistance, \
//                                                                                        cId, \
//                                                                                        cXsq, \
//                                                                                        cYsq, \
//                                                                                        it)
//    {
//#pragma omp for
//        for (i = 0; i < posCount; ++i)
//        {
//            cId = keys.at(i);
//            cPos = positionMap[cId];
//            cXsq = pow((thePos.x() - cPos.x()), 2.0);
//            cYsq = pow((thePos.y() - cPos.y()), 2.0);
//            cDistance = qSqrt(cXsq + cYsq);

//#pragma omp critical
//            {
//                distances.insert(cDistance, cId);
//            }

//#pragma omp critical
//            {
//                it = zv_sortedDistanceMap[cId].find(zv_sortedDistanceMap[cId].key(id), id);
//            }
//            if (it != zv_sortedDistanceMap[cId].end())
//            {
//#pragma omp critical
//                {
//                    zv_sortedDistanceMap[cId].erase(it);
//                }
//            }

//#pragma omp critical
//            {
//                zv_sortedDistanceMap[cId].insert(cDistance, id);
//            }
//        }
//#pragma omp barrier
//    }

//    zv_sortedDistanceMap[id] = distances;
//}
//============================================================
void ZDistanceRepository::zp_removeDistancesForId(quint64 id)
{
    //    zv_sortedDistanceMap.remove(id);

    //    int count = zv_sortedDistanceMap.count();
    //    int t;
    //    QMultiMap<qreal, quint64>::iterator it;

    //    for (t = 0; t < count; ++t)
    //    {
    //        it = zv_sortedDistanceMap[t].find(zv_sortedDistanceMap[t].key(id), id);
    //        if (it != zv_sortedDistanceMap[t].end())
    //        {
    //            zv_sortedDistanceMap[t].erase(it);
    //        }
    //    }
}
//============================================================
//QMultiMap<qreal, quint64> ZDistanceRepository::zh_calcDistanceMapForId(
//    quint64 id, QPointF thePos, const QMap<quint64, QPointF>& positionMap) const
//{
//    QMultiMap<qreal, quint64> distances;
//    int i;
//    qreal cDistance;
//    quint64 cId;
//    qreal cXsq;
//    qreal cYsq;
//    QPointF cPos;
//    QList<quint64> keys = positionMap.keys();
//    keys.removeAt(keys.indexOf(id));
//    int posCount = keys.count();

//#pragma omp parallel shared(distances, \
//                            positionMap, \
//                            keys, \
//                            posCount) private(i, cDistance, cId, cXsq, cYsq)
//    {
//#pragma omp for
//        for (i = 0; i < posCount; ++i)
//        {
//            cId = keys.at(i);
//            cPos = positionMap[cId];
//            cXsq = pow((thePos.x() - cPos.x()), 2.0);
//            cYsq = pow((thePos.y() - cPos.y()), 2.0);
//            cDistance = qSqrt(cXsq + cYsq);

//#pragma omp critical
//            {
//                distances.insert(cDistance, cId);
//            }
//        }
//#pragma omp barrier
//    }

//    return distances;
//}
//============================================================
//bool ZDistanceRepository::zh_calcDistanceForId(quint64 currentId,
//                                               QPointF hostPos,
//                                               qreal& distance) const
//{
//    bool ok = false;
//    QPointF currentPosition;
//    emit zg_inquirePositionForId(currentId, currentPosition, &ok);
//    if (!ok)
//    {
//        return false;
//    }

//    qreal Xsq = pow((hostPos.x() - currentPosition.x()), 2.0);
//    qreal Ysq = pow((hostPos.y() - currentPosition.y()), 2.0);
//    distance = qSqrt(Xsq + Ysq);

//    return true;
//}
//============================================================
//void ZDistanceRepository::zp_sortedDistancesForId(quint64 id,
//                                                  QMultiMap<qreal, quint64>& sortedDistanceMap)
//{
//    sortedDistanceMap = zv_sortedDistanceMap.value(id);
//}
//============================================================
void ZDistanceRepository::zp_clearDistances()
{
    zv_distanceHash.clear();
    //    zv_sortedDistanceMap.clear();
    //    zv_distanceMap.clear();
}
//============================================================
