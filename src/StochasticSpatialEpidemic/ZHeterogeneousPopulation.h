//============================================================
#ifndef ZHETEROGENEOUSPOPULATION_H
#define ZHETEROGENEOUSPOPULATION_H
//============================================================
#include <ZAbstractPopulation.h>
//============================================================
class ZHeterogeneousPopulation : public ZAbstractPopulation
{
    Q_OBJECT
public:
    explicit ZHeterogeneousPopulation(QObject *parent = nullptr);

    void zp_generate();

signals:

};
//============================================================
#endif // ZHETEROGENEOUSPOPULATION_H
