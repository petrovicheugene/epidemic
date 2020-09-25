//============================================================
#include "ZRandom2DPositionGenerator.h"
#include <QPointF>
#include <QRandomGenerator>
//============================================================
ZRandom2DPositionGenerator::ZRandom2DPositionGenerator(QObject* parent) : QObject(parent) {}
//============================================================
QList<QPointF> ZRandom2DPositionGenerator::zp_generate(const QPointF& center,
                                                       double sigma,
                                                       int count) const
{
    qreal highest = 1000;
    QList<QPointF> positionList;
    QRandomGenerator::global()->generateDouble();
    for (int i = 0; i < 10; ++i)
    {
        qreal x = QRandomGenerator::global()->bounded(highest * 2) - highest;
        qreal y = QRandomGenerator::global()->bounded(highest * 2) - highest;
        positionList.append(QPointF(x, y));
    }

    return positionList;
}
//============================================================
