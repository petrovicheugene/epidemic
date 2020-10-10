//============================================================
#ifndef ZDISTANCEREPOSITORY_H
#define ZDISTANCEREPOSITORY_H
//============================================================
#include "ZDistance.h"

#include <QHash>
#include <QMap>
#include <QMetaType>
#include <QObject>
#include <QPointF>
//============================================================
class ZDistanceRepository : public QObject
{
    Q_OBJECT
public:
    explicit ZDistanceRepository(QObject* parent = nullptr);

public slots:

    // +++
    void zp_recalcDistancesForPositions(const QHash<quint64, QPointF>& positionHash);
    // ***
    void zp_recalcDistancesForPosition(quint64 id, QPointF pos);

    void zp_removeDistancesForId(quint64 id);
    void zp_clearDistances();

signals:

    //void zg_inquirePositionForId(quint64 id, QPointF& pos, bool* ok = nullptr) const;
    //void zg_inquireIndividualPositionMap(QMap<quint64, QPointF>& positionMap) const;

    // +++
    void zg_inquireIndividualPositionHash(QHash<quint64, QPointF>& positionHash) const;
    // ***

private:
    // VARS
    //    QMap<quint64, QMultiMap<qreal, quint64>> zv_sortedDistanceMap;
    //    QMap<quint64, QList<ZDistance>> zv_distanceMap;

    // FUNCS
    //    bool zh_calcDistanceForId(quint64 currentId, QPointF hostPos, qreal& distance) const;
    //    QMultiMap<qreal, quint64> zh_calcDistanceMapForId(
    //        quint64 theId, QPointF pos, const QMap<quint64, QPointF>& positionMap) const;
    //    void zh_recalcSortedDistanceMapForId(quint64 theId,
    //                                         const QMap<qreal, quint64>& distansesForTheId);

    //    void zh_recalcSortedDistanceMapForId(quint64 id,
    //                                         QPointF thePos,
    //                                         const QMap<quint64, QPointF>& positionMap);

    // +++
    // VARS
    QHash<quint64, QList<ZDistance>> zv_distanceHash;

    // FUNCS
    //    void zh_calcDistancesForPos(const QPointF& pos, QList<ZDistance>& distancesToPos) const;
    QHash<quint64, QHash<quint64, qreal>> zh_distancesSortedById() const;
    void zh_convertToDistanceHash(const QHash<quint64, QHash<quint64, qreal>>& distances);
};
//============================================================
#endif // ZDISTANCEREPOSITORY_H
