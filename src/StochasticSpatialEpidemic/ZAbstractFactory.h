//============================================================
#ifndef ZABSTRACTFACTORY_H
#define ZABSTRACTFACTORY_H
//============================================================
#include "ZAbstractDashBoard.h"
#include "ZAbstractEpidemicProcess.h"
#include "ZAbstractPopulation.h"
#include "ZAbstractPopulationWidget.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
//============================================================
class ZAbstractFactory : public QObject
{
    Q_OBJECT
public:
    explicit ZAbstractFactory(QObject* parent = nullptr);
    virtual ZAbstractPopulation* zp_createPopulation() const = 0;
    virtual ZAbstractEpidemicProcess* zp_createEpidemicProcess() const = 0;
    virtual ZAbstractDashBoard* zp_createPopulationDashBoard() const = 0;
    virtual ZAbstractDashBoard* zp_createEpidemicProcessDashBoard() const = 0;
    virtual ZAbstractPopulationWidget* zp_createPopulationWidget() const = 0;
};
//============================================================
#endif // ZABSTRACTFACTORY_H
