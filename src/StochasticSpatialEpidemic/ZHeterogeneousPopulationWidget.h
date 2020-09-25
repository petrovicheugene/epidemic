//============================================================
#ifndef ZHETEROGENEOUSPOPULATIONVIEW_H
#define ZHETEROGENEOUSPOPULATIONVIEW_H
//============================================================
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

    void zg_queryIndividualPosition(quint64 id, QPointF& position) const;
    void zg_queryIndividualHealthState(quint64 id, int& healthState) const;
    void zg_individualItemMousePressed(quint64 id) const;
    void zg_individualItemHealthStateChanged(quint64 id, int& healthState) const;

public slots:

    void zp_addIndividualItem(quint64 id);
    void zp_removeIndividualItem(quint64 id) const;
    void zp_setIndividualItemPosition(quint64 id, QPointF position) const;
    void zp_setIndividualItemHealth(quint64 id, int healthState) const;

private slots:
    void zh_onIndividualItemMousePress(quint64 id) const;

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
