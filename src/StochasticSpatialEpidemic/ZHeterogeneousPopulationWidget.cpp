//============================================================
#include "ZHeterogeneousPopulationWidget.h"
#include "ZHeterogeneousPopulation.h"
#include "ZHeterogeneousPopulationScene.h"
#include "ZHeterogeneousPopulationView.h"
#include "ZPositionedIndividualGraphicsItem.h"

#include <tuple>
#include <QDebug>
#include <QElapsedTimer>
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
            &ZHeterogeneousPopulation::zg_individualListAdded,
            this,
            &ZHeterogeneousPopulationWidget::zp_addIndividualItemForSpecList);
    connect(population,
            &ZHeterogeneousPopulation::zg_individualRemoved,
            this,
            &ZHeterogeneousPopulationWidget::zp_removeIndividualItem);
    connect(population,
            &ZHeterogeneousPopulation::zg_allIindividualsRemoved,
            this,
            &ZHeterogeneousPopulationWidget::zp_removeAllItems);
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

    //    connect(this,
    //            &ZHeterogeneousPopulationWidget::zg_inquireRecoveryProbability,
    //            population,
    //            &ZHeterogeneousPopulation::zp_recoveryProbabilityForId);
    //    connect(this,
    //            &ZHeterogeneousPopulationWidget::zg_invokeSetRecoveryProbability,
    //            population,
    //            &ZHeterogeneousPopulation::zp_setRecoveryProbabilityForId);

    return true;
}
//============================================================
void ZHeterogeneousPopulationWidget::zp_addIndividualItem(quint64 id)
{
    if (zh_itemForIndex(id))
    {
        return;
    }

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
void ZHeterogeneousPopulationWidget::zp_addIndividualItemForSpec(
    std::tuple<quint64, QPointF, HealthStatus> tuple)
{
    quint64 id = std::get<0>(tuple);

    if (zh_itemForIndex(id))
    {
        return;
    }

    QPointF position = std::get<1>(tuple);
    HealthStatus healthStatus = std::get<2>(tuple);
    ZPositionedIndividualGraphicsItem* item = new ZPositionedIndividualGraphicsItem(id);
    connect(item,
            &ZPositionedIndividualGraphicsItem::zg_pressed,
            this,
            &ZHeterogeneousPopulationWidget::zh_onIndividualItemMousePress);
    item->setPos(position);
    item->zp_setItemHealthState(healthStatus);
    zv_scene->addItem(item);

    QRectF rect = zv_scene->itemsBoundingRect();
    if (!zv_scene->sceneRect().contains(rect))
    {
        zv_scene->setSceneRect(rect);
    }
}
//============================================================
void ZHeterogeneousPopulationWidget::zp_addIndividualItemForSpecList(
    QList<std::tuple<quint64, QPointF, HealthStatus>> itemList)
{
    for (int i = 0; i < itemList.count(); ++i)
    {
        std::tuple<quint64, QPointF, HealthStatus> tuple = itemList.at(i);
        zp_addIndividualItemForSpec(tuple);
    }
}
//============================================================
void ZHeterogeneousPopulationWidget::zh_onIndividualItemMousePress(quint64 id) const
{
    emit zg_individualItemMousePressed(id);
    emit zg_individualItemHealthStateChanged(id, HS_INFECTIOUS);
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
    delete item;
}
//============================================================
void ZHeterogeneousPopulationWidget::zp_removeAllItems() const
{
    foreach (auto item, zv_scene->items())
    {
        zv_scene->removeItem(item);
        delete item;
    }

    zv_scene->setSceneRect(QRectF(QPointF(0, 0), QSizeF(0.1, 0.1)));
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
