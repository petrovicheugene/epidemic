//============================================================
#ifndef ZEPIDEMICPROCESS_H
#define ZEPIDEMICPROCESS_H
//============================================================
#include "ZStochasticHeterogeneousProcessCommon.h"
#include <QObject>
#include <QVariant>

//============================================================
class ZAbstractPopulation;
//============================================================
class ZAbstractEpidemicProcess : public QObject
{
    Q_OBJECT
public:
    explicit ZAbstractEpidemicProcess(QObject* parent = nullptr);
    virtual void zp_setPopulation(ZAbstractPopulation* population) = 0;
    virtual QStringList zp_groupList() const = 0;

public slots:

    virtual void zp_dispatchCommand(int commandId, QVariant data = QVariant()) = 0;
    virtual void zp_processStep(int&) const = 0;

signals:

    void zg_processChanged(int status, QVariant data = QVariant()) const;
    void zg_epidemicFinished() const;

private:
    // VARS

    // FUNCS
};
//============================================================
#endif // ZEPIDEMICPROCESS_H
