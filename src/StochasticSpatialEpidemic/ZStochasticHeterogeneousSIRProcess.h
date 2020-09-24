//============================================================
#ifndef ZSIREPIDEMICPROCESS_H
#define ZSIREPIDEMICPROCESS_H
//============================================================
#include "ZAbstractEpidemicProcess.h"
//============================================================
class ZStochasticHeterogeneousSIRProcess : public ZAbstractEpidemicProcess
{
    Q_OBJECT
public:
    explicit ZStochasticHeterogeneousSIRProcess(QObject* parent = nullptr);

    void zp_setPopulation(ZAbstractPopulation* population) override;
    QStringList zp_groupList() const override;

public slots:

    void zp_start() override;
    void zp_pause() override;
    void zp_stop() override;

signals:

    void zg_groupSizeChanged(const QString& groupName, int size) const;
    void zg_finished() const;

private:
    // VARS

    // FUNCS
};
//============================================================
#endif // ZSIREPIDEMICPROCESS_H
