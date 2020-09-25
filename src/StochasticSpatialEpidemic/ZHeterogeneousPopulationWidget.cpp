//============================================================
#include "ZHeterogeneousPopulationWidget.h"
#include "ZHeterogeneousPopulation.h"
#include "ZHeterogeneousPopulationScene.h"
#include "ZHeterogeneousPopulationView.h"
#include "ZPositionedIndividualGraphicsItem.h"

#include <QVBoxLayout>
//============================================================
ZHeterogeneousPopulationWidget::ZHeterogeneousPopulationWidget(QWidget* parent)
    : ZAbstractPopulationWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//============================================================
void ZHeterogeneousPopulationWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    zv_view = new ZHeterogeneousPopulationView;
    mainLayout->addWidget(zv_view);
    zv_scene = new ZHeterogeneousPopulationScene(this);
}
//============================================================
void ZHeterogeneousPopulationWidget::zh_createConnections()
{
    zv_view->zp_setScene(zv_scene);
}
//============================================================
bool ZHeterogeneousPopulationWidget::zp_setPopulation(ZAbstractPopulation* abstractPopulation)
{
    ZHeterogeneousPopulation* population = qobject_cast<ZHeterogeneousPopulation*>(
        abstractPopulation);

    if (!population)
    {
        return false;
    }

    connect(population,
            &ZHeterogeneousPopulation::zg_individualAdded,
            this,
            &ZHeterogeneousPopulationWidget::zp_addIndividualItem);
    connect(population,
            &ZHeterogeneousPopulation::zg_individualRemoved,
            this,
            &ZHeterogeneousPopulationWidget::zp_removeIndividualItem);
    connect(population,
            &ZHeterogeneousPopulation::zg_individualPositionChanged,
            this,
            &ZHeterogeneousPopulationWidget::zp_setIndividualItemPosition);
    connect(population,
            &ZHeterogeneousPopulation::zg_individualHealthChanged,
            this,
            &ZHeterogeneousPopulationWidget::zp_setIndividualItemHealth);

    connect(this,
            &ZHeterogeneousPopulationWidget::zg_queryIndividualPosition,
            population,
            &ZHeterogeneousPopulation::zp_individualPosition);
    connect(this,
            &ZHeterogeneousPopulationWidget::zg_queryIndividualHealthState,
            population,
            &ZHeterogeneousPopulation::zp_individualHealthState);
    connect(this,
            &ZHeterogeneousPopulationWidget::zg_individualItemHealthStateChanged,
            population,
            &ZHeterogeneousPopulation::zp_setIndividualHealth);
    return true;
}
//============================================================
void ZHeterogeneousPopulationWidget::zp_addIndividualItem(quint64 id)
{
    QPointF position;
    int healthState;

    emit zg_queryIndividualPosition(id, position);
    emit zg_queryIndividualHealthState(id, healthState);

    ZPositionedIndividualGraphicsItem* item = new ZPositionedIndividualGraphicsItem(id);
    connect(item,
            &ZPositionedIndividualGraphicsItem::zg_pressed,
            this,
            &ZHeterogeneousPopulationWidget::zh_onIndividualItemMousePress);
    item->setPos(position);
    item->zp_setItemHealthState(healthState);
    zv_scene->addItem(item);
    zv_view->fitInView(zv_scene->sceneRect());
}
//============================================================
void ZHeterogeneousPopulationWidget::zh_onIndividualItemMousePress(quint64 id) const
{
    emit zg_individualItemMousePressed(id);
    int infectiousState = ZPositionedIndividualGraphicsItem::HS_INFECTIOUS;

    emit zg_individualItemHealthStateChanged(id, infectiousState);
}
//============================================================
void ZHeterogeneousPopulationWidget::zp_removeIndividualItem(quint64 id) const {}
//============================================================
void ZHeterogeneousPopulationWidget::zp_setIndividualItemPosition(quint64 id, QPointF position) const
{
    ZPositionedIndividualGraphicsItem* item = zh_itemForIndex(id);
    if (!item || item->pos() == position)
    {
        return;
    }

    item->setPos(position);
}
//============================================================
void ZHeterogeneousPopulationWidget::zp_setIndividualItemHealth(quint64 id, int healthState) const
{
    ZPositionedIndividualGraphicsItem* item = zh_itemForIndex(id);
    if (!item)
    {
        return;
    }

    item->zp_setItemHealthState(healthState);
}
//============================================================
ZPositionedIndividualGraphicsItem* ZHeterogeneousPopulationWidget::zh_itemForIndex(quint64 id) const
{
    ZPositionedIndividualGraphicsItem* item = nullptr;

    QList<QGraphicsItem*> itemList = zv_scene->items();
    foreach (auto graphicsItem, itemList)
    {
        item = qgraphicsitem_cast<ZPositionedIndividualGraphicsItem*>(graphicsItem);
        if (item && item->zp_id() == id)
        {
            return item;
        }
    }

    return nullptr;
}
//============================================================
