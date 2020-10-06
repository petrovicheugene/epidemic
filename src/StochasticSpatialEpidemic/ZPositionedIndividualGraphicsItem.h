//============================================================
#ifndef ZINDIVIDUALGRAPHICSITEM_H
#define ZINDIVIDUALGRAPHICSITEM_H
//============================================================
#include "ZStochasticHeterogeneousProcessCommon.h"
#include <QGraphicsObject>
//============================================================
class ZPositionedIndividualGraphicsItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ZPositionedIndividualGraphicsItem(quint64 id, QGraphicsItem* parent = nullptr);
    void zp_setItemHealthState(int);
    quint64 zp_id();
    // static
    static void zp_setColor(HealthStatus state, QColor color);
    static void zp_setSize(qreal size);

signals:

    void zg_pressed(quint64 id) const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

private:
    // VARS
    // static
    static QMap<HealthStatus, QColor> zv_colorMap;
    static qreal zv_size;
    quint64 zv_id;
    QColor zv_brushColor;

    // FUNCS
    static QMap<HealthStatus, QColor> initColorMap();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
};
//============================================================
#endif // ZINDIVIDUALGRAPHICSITEM_H
