//============================================================
#ifndef ZABSTRACTPOPULATIONWIDGET_H
#define ZABSTRACTPOPULATIONWIDGET_H
//============================================================
#include <QWidget>
//============================================================
class ZAbstractPopulation;
//============================================================
class ZAbstractPopulationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZAbstractPopulationWidget(QWidget *parent = nullptr);
    virtual bool zp_setPopulation(ZAbstractPopulation* population) = 0;

signals:

};
//============================================================
#endif // ZABSTRACTPOPULATIONWIDGET_H
