//============================================================
#include "ZRandom2DPositionGenerator.h"
#include <QPointF>
//============================================================
ZRandom2DPositionGenerator::ZRandom2DPositionGenerator(QObject* parent) : QObject(parent) {}
//============================================================
QList<QPointF> ZRandom2DPositionGenerator::zp_generate(const QPointF& center,
                                                       double sigma,
                                                       int count) const
{
    QList<QPointF> positionList;

    for (int i = 0; i < 10; ++i)
    {
        positionList.append(QPointF(i, i * 2));
    }

    return positionList;
}
//============================================================
