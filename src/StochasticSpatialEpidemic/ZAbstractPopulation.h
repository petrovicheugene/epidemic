//============================================================
#ifndef ZABSTRACTPOPULATION_H
#define ZABSTRACTPOPULATION_H
//============================================================
#include <QObject>
//============================================================
class ZAbstractPopulation : public QObject
{
    Q_OBJECT
public:
    explicit ZAbstractPopulation(QObject* parent = nullptr);

    virtual void zp_generate() = 0;

signals:

};
//============================================================
#endif // ZABSTRACTPOPULATION_H
