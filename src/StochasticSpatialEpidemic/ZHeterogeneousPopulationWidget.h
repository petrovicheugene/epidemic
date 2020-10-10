//============================================================
#ifndef ZHETEROGENEOUSPOPULATIONVIEW_H
#define ZHETEROGENEOUSPOPULATIONVIEW_H
//============================================================
#include "ZStochasticHeterogeneousProcessCommon.h"
#include <ZAbstractPopulationWidget.h>
//============================================================
class ZHeterogeneousPopulationScene;
class ZHeterogeneousPopulationView;
class ZPositionedIndividualGraphicsItem;
//============================================================
class ZHeterogeneousPopulationWidget : public ZAbstractPopulationWidget
{
    Q_OBJECT
public:
    explicit ZHeterogeneousPopulationWidget(QWidget* parent = nullptr);
    bool zp_setPopulation(ZAbstractPopulation* abstractPopulation) override;

signals:

    void zg_queryIndividualPosition(quint64 id, QPointF& position, bool* ok = nullptr) const;
    void zg_queryIndividualHealthState(quint64 id,
                                       HealthStatus& healthState,
                                       bool* ok = nullptr) const;
    void zg_individualItemMousePressed(quint64 id, bool* ok = nullptr) const;
    void zg_individualItemHealthStateChanged(quint64 id,
                                             HealthStatus healthState,
                                             bool* ok = nullptr) const;
    void zg_inquireRecoveryProbability(quint64 id, qreal& healthState, bool* ok = nullptr) const;
    void zg_invokeSetRecoveryProbability(quint64 id, qreal healthState, bool* ok = nullptr) const;
    void zg_positionMarked(QPointF) const;

public slots:

    void zp_addIndividualItem(quint64 id);
    void zp_addIndividualItemForSpec(std::tuple<quint64, QPointF, HealthStatus>);
    void zp_addIndividualItemForSpecList(QList<std::tuple<quint64, QPointF, HealthStatus>> itemList);
    void zp_removeIndividualItem(quint64 id) const;
    void zp_removeAllItems() const;
    void zp_setIndividualItemPosition(quint64 id, QPointF position) const;
    void zp_setIndividualItemHealth(quint64 id, HealthStatus healthState) const;
    void zp_setPlotRect(QRectF rect) const;
    void zp_setMarkSize(int val) const;
    void zp_fitInView() const;

private slots:

    void zh_onIndividualItemMousePress(quint64 id) const;

protected:
    bool eventFilter(QObject* object, QEvent* event) override;

private:
    // VARS
    ZHeterogeneousPopulationScene* zv_scene;
    ZHeterogeneousPopulationView* zv_view;
    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    ZPositionedIndividualGraphicsItem* zh_itemForIndex(quint64 id) const;
};
//============================================================
#endif // ZHETEROGENEOUSPOPULATIONVIEW_H
