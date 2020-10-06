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
    void zp_onPositionChange1(quint64 id, QPointF pos);

    void zp_distancesForId(quint64 id, QMap<quint64, qreal>& distanceMap);

signals:

    void zg_inquirePositionForId(quint64 id, QPointF& pos, bool* ok = nullptr) const;

private:
    // VARS
    QMap<quint64, QMap<quint64, qreal>> zv_distanceMap;
    QMap<quint64, QMap<quint64, qreal>> zv_infectionProbabilityMap;

    // FUNCS
    bool zh_calcDistanceForId(quint64 currentId, QPointF hostPos, qreal& distance) const;
};
//============================================================
#endif // ZDISTANCEREPOSITORY_H
