//============================================================
#ifndef ZRECOVERYPROBABILITYCALCULATOR_H
#define ZRECOVERYPROBABILITYCALCULATOR_H
//============================================================
#include <QObject>
//============================================================
class ZRecoveryProbabilityCalculator : public QObject
{
    Q_OBJECT
public:
    explicit ZRecoveryProbabilityCalculator(QObject* parent = nullptr);

    qreal zp_calcProbability(qreal probability) const;

public slots:

    void zp_setStartingProbability(qreal probability);
    void zp_startingProbability(qreal& probability) const;
    void zp_setRecoveryDurationFactor(qreal factor);
    void zp_recoveryDurationFactor(qreal& factor) const;

signals:

private:
    // VARS
    qreal zv_startingProbability;
    qreal zv_recoveryDurationFactor;
};
//============================================================
#endif // ZRECOVERYPROBABILITYCALCULATOR_H
