//============================================================
#ifndef ZHETEROGENEOUSPOPULATION_H
#define ZHETEROGENEOUSPOPULATION_H
//============================================================
#include <ZAbstractPopulation.h>
#include <QMap>
#include <QPointF>
//============================================================
class ZPositionedIndividual;
//============================================================
class ZHeterogeneousPopulation : public ZAbstractPopulation
{
    Q_OBJECT
public:
    explicit ZHeterogeneousPopulation(QObject* parent = nullptr);

public slots:

    void zp_generate();
    void zp_individualPosition(quint64 id, QPointF& position) const;
    void zp_individualHealthState(quint64 id, int& healthState) const;

    void zp_setIndividualHealth(quint64 id, int healthState);

signals:

    void zg_individualAdded(quint64 id) const;
    void zg_individualRemoved(quint64 id) const;

    void zg_individualPositionChanged(quint64 id, QPointF position) const;
    void zg_individualHealthChanged(quint64 id, int healthState) const;

private slots:

private:
    // VARS
    QMap<quint64, ZPositionedIndividual*> zv_individuals;
    // FUNCS
};
//============================================================
#endif // ZHETEROGENEOUSPOPULATION_H
