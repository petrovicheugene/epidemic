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

    void zp_recalcDistancesForPositions(const QHash<quint64, QPointF>& positionHash);
    void zp_recalcDistancesForPosition(quint64 id, QPointF position);
    void zp_clearDistances();
    void zp_distancesForId(quint64 id, QList<ZDistance>& distances) const;

signals:

    void zg_inquireIndividualPositionHash(QHash<quint64, QPointF>& positionHash) const;

private:
    // VARS
    QHash<quint64, QList<ZDistance>> zv_distanceHash;

    // FUNCS
    QHash<quint64, QHash<quint64, qreal>> zh_cacheFromWorkDistances() const;
    void zh_convertToWorkDistances(const QHash<quint64, QHash<quint64, qreal>>& distances);
};
//============================================================
#endif // ZDISTANCEREPOSITORY_H
