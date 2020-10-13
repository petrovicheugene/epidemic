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
    // convertation
    QHash<quint64, QHash<quint64, qreal>> cache = zh_cacheFromWorkDistances();
    // inquire the existing positions
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

                //#pragma omp critical
                {
                    cache[nId][eId] = aDistance;
                    cache[eId][nId] = aDistance;
                }
            }
        }
    }

    zh_convertToWorkDistances(cache);
}
//============================================================
void ZDistanceRepository::zp_recalcDistancesForPosition(quint64 id, QPointF position)
{
    QHash<quint64, QPointF> positionHash;
    positionHash.insert(id, position);
    zp_recalcDistancesForPositions(positionHash);
}
//============================================================
QHash<quint64, QHash<quint64, qreal>> ZDistanceRepository::zh_cacheFromWorkDistances() const
{
    QElapsedTimer timer;
    timer.start();

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
void ZDistanceRepository::zh_convertToWorkDistances(
    const QHash<quint64, QHash<quint64, qreal>>& distances)
{
    QHash<quint64, QList<ZDistance>> distanceCache;
    QList<ZDistance> distanceList;

    QHash<quint64, QHash<quint64, qreal>>::const_iterator it;
    QHash<quint64, qreal>::const_iterator rowIt;
    for (it = distances.begin(); it != distances.end(); ++it)
    {
        distanceList.clear();
        for (rowIt = it.value().begin(); rowIt != it.value().end(); ++rowIt)
        {
            distanceList.append(ZDistance(rowIt.key(), rowIt.value()));
        }
        std::sort(distanceList.begin(), distanceList.end());
        distanceCache[it.key()] = distanceList;
    }

    zv_distanceHash = distanceCache;
}
//============================================================
void ZDistanceRepository::zp_distancesForId(quint64 id, QList<ZDistance>& distances) const
{
    distances = zv_distanceHash.value(id, QList<ZDistance>());
}
//============================================================
void ZDistanceRepository::zp_clearDistances()
{
    zv_distanceHash.clear();
}
//============================================================
