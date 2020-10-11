//============================================================
#include "ZPositionedIndividualGraphicsItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
//============================================================
// STATIC
//============================================================
QMap<HealthStatus, QColor> ZPositionedIndividualGraphicsItem::zv_colorMap
    = ZPositionedIndividualGraphicsItem::initColorMap();
qreal ZPositionedIndividualGraphicsItem::zv_size = 3;
//============================================================
QMap<HealthStatus, QColor> ZPositionedIndividualGraphicsItem::initColorMap()
{
    QMap<HealthStatus, QColor> colorMap;
    colorMap.insert(HS_NOT_DEFINED, Qt::transparent);
    colorMap.insert(HS_SUSCEPTIBLE, Qt::blue);
    colorMap.insert(HS_INFECTIOUS, Qt::red);
    colorMap.insert(HS_RECOVERED, Qt::green);
    return colorMap;
}
//============================================================
void ZPositionedIndividualGraphicsItem::zp_setColor(HealthStatus state, QColor color)
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
    zv_brushColor = zv_colorMap.value(static_cast<HealthStatus>(healthState));
    update();
}
//============================================================
QRectF ZPositionedIndividualGraphicsItem::boundingRect() const
{
    qreal half = zv_size / 2.0;
    QRectF rect(QPointF(half, half), QSizeF(zv_size, zv_size));
    rect.moveCenter(pos());
    return rect;
}
//============================================================
void ZPositionedIndividualGraphicsItem::paint(QPainter* painter,
                                              const QStyleOptionGraphicsItem* option,
                                              QWidget* widget)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->setPen(QPen(QBrush(Qt::black), 0.5));

    painter->setBrush(zv_brushColor);
    painter->drawEllipse(boundingRect());
    painter->restore();

    Q_UNUSED(option);
    Q_UNUSED(widget);
}
//============================================================
