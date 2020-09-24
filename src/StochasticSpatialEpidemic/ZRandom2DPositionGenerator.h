//============================================================
#ifndef ZNEIGHBORGENERATOR_H
#define ZNEIGHBORGENERATOR_H
//============================================================
#include <QObject>
//============================================================
class ZRandom2DPositionGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ZRandom2DPositionGenerator(QObject* parent = nullptr);

    QList<QPointF> zp_generate(const QPointF& center, double sigma, int count) const;

signals:
};
//============================================================
#endif // ZNEIGHBORGENERATOR_H
