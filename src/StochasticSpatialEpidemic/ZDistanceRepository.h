//============================================================
#ifndef ZDISTANCEREPOSITORY_H
#define ZDISTANCEREPOSITORY_H
//============================================================
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

    void zp_onPositionChange(quint64 id, QPointF pos);
    void zp_recalcSortedDistanceMap();
    void zp_removeDistancesForId(quint64 id);
    void zp_sortedDistancesForId(quint64 id, QMultiMap<qreal, quint64>& sortedDistanceMap);
    void zp_clearDistances();

signals:

    void zg_inquirePositionForId(quint64 id, QPointF& pos, bool* ok = nullptr) const;
    void zg_inquireIndividualPositionMap(QMap<quint64, QPointF>& positionMap) const;

private:
    // VARS
    QMap<quint64, QMultiMap<qreal, quint64>> zv_sortedDistanceMap;

    // FUNCS
    bool zh_calcDistanceForId(quint64 currentId, QPointF hostPos, qreal& distance) const;
    QMultiMap<qreal, quint64> zh_calcDistanceMapForId(
        quint64 theId, QPointF pos, const QMap<quint64, QPointF>& positionMap) const;
    void zh_recalcSortedDistanceMapForId(quint64 theId,
                                         const QMap<qreal, quint64>& distansesForTheId);

    void zh_recalcSortedDistanceMapForId(quint64 id,
                                         QPointF thePos,
                                         const QMap<quint64, QPointF>& positionMap);
};
//============================================================
#endif // ZDISTANCEREPOSITORY_H
