//============================================================
#include "ZRandom2DPositionGenerator.h"
#include "ZGenerationSettings.h"

#include <QDateTime>
#include <QDebug>
#include <QPointF>
#include <QRandomGenerator>
#include <QRectF>
//============================================================
ZRandom2DPositionGenerator::ZRandom2DPositionGenerator(QObject* parent) : QObject(parent) {}
//============================================================
QList<QPointF> ZRandom2DPositionGenerator::zp_generate(ZGenerationSettings settings) const
{
    bool ok;
    int count = settings.zp_parameter(ZGenerationSettings::PN_GENERATION_SIZE).toInt(&ok);

    if (!ok)
    {
        return QList<QPointF>();
    }

    QList<QPointF> positionList;
    if (settings.zp_distributionType() == ZGenerationSettings::DT_UNIFORM)
    {
        QRectF rect = settings.zp_parameter(ZGenerationSettings::PN_PLOT_RECT).toRectF();
        for (int i = 0; i < count; ++i)
        {
            qreal x = QRandomGenerator::global()->bounded(rect.height());
            qreal y = QRandomGenerator::global()->bounded(rect.width());
            positionList.append(QPointF(x, y));
        }
    }
    else if (settings.zp_distributionType() == ZGenerationSettings::DT_NORMAL)
    {
        qreal dispersion = settings.zp_parameter(ZGenerationSettings::PN_DISPERSION).toDouble();
        QPointF center = settings.zp_parameter(ZGenerationSettings::PN_DISPERSION_CENTER).toPointF();

        //        std::default_random_engine generatorX;
        //        generatorX.seed(QDateTime::currentMSecsSinceEpoch());
        //        std::default_random_engine generatorY;
        //        generatorY.seed(QDateTime::currentMSecsSinceEpoch());

        std::default_random_engine generator;
        generator.seed(QDateTime::currentMSecsSinceEpoch());
        std::normal_distribution<double> distributionX(center.x(), dispersion);
        std::normal_distribution<double> distributionY(center.y(), dispersion);

        for (int i = 0; i < count; ++i)
        {
            qreal x = distributionX(generator);
            qreal y = distributionY(generator);

            positionList.append(QPointF(x, y));
        }
    }

    return positionList;
}
//============================================================
