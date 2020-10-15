//============================================================
#include "ZHeterogeneousPopulationView.h"
#include <QDebug>
#include <QWheelEvent>
//============================================================
ZHeterogeneousPopulationView::ZHeterogeneousPopulationView(QWidget* parent) : QGraphicsView(parent)
{
    // setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}
//============================================================
void ZHeterogeneousPopulationView::zp_setScene(QGraphicsScene* scene)
{
    QGraphicsView::setScene(scene);
}
//============================================================
void ZHeterogeneousPopulationView::wheelEvent(QWheelEvent* event)
{
    double factor = 0.0;

    if (event->angleDelta().y() < 0)
    {
        factor = 0.9;
    }
    else
    {
        factor = 1.1;
    }

    scale(factor, factor);
}
//============================================================
