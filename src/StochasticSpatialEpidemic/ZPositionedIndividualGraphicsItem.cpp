//============================================================
#include "ZPositionedIndividualGraphicsItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
//============================================================
// STATIC
//============================================================
QMap<ZPositionedIndividualGraphicsItem::HealthState, QColor>
    ZPositionedIndividualGraphicsItem::zv_colorMap
    = ZPositionedIndividualGraphicsItem::initColorMap();
qreal ZPositionedIndividualGraphicsItem::zv_size = 5;
//============================================================
QMap<ZPositionedIndividualGraphicsItem::HealthState, QColor>
ZPositionedIndividualGraphicsItem::initColorMap()
{
    QMap<ZPositionedIndividualGraphicsItem::HealthState, QColor> colorMap;
    colorMap.insert(ZPositionedIndividualGraphicsItem::HS_NOT_DEFINED, Qt::transparent);
    colorMap.insert(ZPositionedIndividualGraphicsItem::HS_SUSCEPTIBLE, Qt::cyan);
    colorMap.insert(ZPositionedIndividualGraphicsItem::HS_INFECTIOUS, Qt::red);
    colorMap.insert(ZPositionedIndividualGraphicsItem::HS_RECOVERED, Qt::green);
    return colorMap;
}
//============================================================
void ZPositionedIndividualGraphicsItem::zp_setColor(
    ZPositionedIndividualGraphicsItem::HealthState state, QColor color)
{
    zv_colorMap[state] = color;
}
//============================================================
void ZPositionedIndividualGraphicsItem::zp_setSize(qreal size)
{
    ZPositionedIndividualGraphicsItem::zv_size = size;
}
//============================================================
// END STATIC
//============================================================
ZPositionedIndividualGraphicsItem::ZPositionedIndividualGraphicsItem(quint64 id,
                                                                     QGraphicsItem* parent)
    : QGraphicsObject(parent)
{
    zv_id = id;

    zv_brushColor = zv_colorMap.value(HS_NOT_DEFINED);
    setFlags(ItemIgnoresTransformations);
}
//============================================================
void ZPositionedIndividualGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    emit zg_pressed(zv_id);
}
//============================================================
quint64 ZPositionedIndividualGraphicsItem::zp_id()
{
    return zv_id;
}
//============================================================
void ZPositionedIndividualGraphicsItem::zp_setItemHealthState(int healthState)
{
    zv_brushColor = zv_colorMap.value(static_cast<HealthState>(healthState));
    update();
}
//============================================================
QRectF ZPositionedIndividualGraphicsItem::boundingRect() const
{
    qreal half = zv_size / 2.0;
    return QRectF(QPointF(half, half), QSizeF(zv_size, zv_size));
}
//============================================================
void ZPositionedIndividualGraphicsItem::paint(QPainter* painter,
                                              const QStyleOptionGraphicsItem* option,
                                              QWidget* widget)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setBrush(zv_brushColor);
    painter->setPen(QPen(QBrush(Qt::black), 0.5));
    painter->drawEllipse(boundingRect());
    painter->restore();

    Q_UNUSED(option);
    Q_UNUSED(widget);
}
//============================================================
