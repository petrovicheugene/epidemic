//============================================================
#include "ZHeterogeneousPopulationWidget.h"
#include "ZHeterogeneousPopulation.h"
#include "ZHeterogeneousPopulationScene.h"
#include "ZHeterogeneousPopulationView.h"
#include "ZPositionedIndividualGraphicsItem.h"

#include <QDebug>
#include <QMouseEvent>
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
    zv_view->viewport()->installEventFilter(this);
    mainLayout->addWidget(zv_view);
    zv_scene = new ZHeterogeneousPopulationScene(this);
}
//============================================================
bool ZHeterogeneousPopulationWidget::eventFilter(QObject* object, QEvent* event)
{
    if (object == zv_view->viewport() && event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        emit zg_positionMarked(zv_view->mapToScene(mouseEvent->pos()));
    }
    return false;
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
            &ZHeterogeneousPopulation::zp_healthStateForId);
    connect(this,
            &ZHeterogeneousPopulationWidget::zg_individualItemHealthStateChanged,
            population,
            &ZHeterogeneousPopulation::zp_setHealthStateForId);

    connect(this,
            &ZHeterogeneousPopulationWidget::zg_inquireRecoveryProbability,
            population,
            &ZHeterogeneousPopulation::zp_recoveryProbabilityForId);
    connect(this,
            &ZHeterogeneousPopulationWidget::zg_invokeSetRecoveryProbability,
            population,
            &ZHeterogeneousPopulation::zp_setRecoveryProbabilityForId);

    return true;
}
//============================================================
void ZHeterogeneousPopulationWidget::zp_addIndividualItem(quint64 id)
{
    QPointF position;
    HealthStatus healthState;

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
}
//============================================================
void ZHeterogeneousPopulationWidget::zh_onIndividualItemMousePress(quint64 id) const
{
    emit zg_individualItemMousePressed(id);
    int infectiousState = HS_INFECTIOUS;

    emit zg_individualItemHealthStateChanged(id, infectiousState);
}
//============================================================
void ZHeterogeneousPopulationWidget::zp_removeIndividualItem(quint64 id) const
{
    ZPositionedIndividualGraphicsItem* item = zh_itemForIndex(id);
    if (!item)
    {
        return;
    }

    zv_scene->removeItem(item);
}
//============================================================
void ZHeterogeneousPopulationWidget::zp_setIndividualItemPosition(quint64 id, QPointF position) const
{
    ZPositionedIndividualGraphicsItem* item = zh_itemForIndex(id);
    if (!item || item->pos() == position)
    {
        return;
    }

    item->setPos(position);
    // zv_scene->update();
    QRectF rect = zv_scene->itemsBoundingRect();
    if (!rect.contains(item->pos()))
    {
        zv_scene->setSceneRect(rect);
    }
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
void ZHeterogeneousPopulationWidget::zp_setPlotRect(QRectF rect) const
{
    zv_scene->setSceneRect(rect);
}
//============================================================
void ZHeterogeneousPopulationWidget::zp_fitInView() const
{
    zv_view->fitInView(zv_scene->sceneRect());
}
//============================================================
void ZHeterogeneousPopulationWidget::zp_setMarkSize(int val) const
{
    ZPositionedIndividualGraphicsItem::zp_setSize(static_cast<qreal>(val));
    zv_scene->update();
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
