//============================================================
#ifndef ZNEIGHBORGENERATOR_H
#define ZNEIGHBORGENERATOR_H
//============================================================
#include "ZGenerationSettings.h"
#include <QObject>

//============================================================
class ZRandom2DPositionGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ZRandom2DPositionGenerator(QObject* parent = nullptr);

    QList<QPointF> zp_generate(ZGenerationSettings settings) const;

signals:
};
//============================================================
#endif // ZNEIGHBORGENERATOR_H
