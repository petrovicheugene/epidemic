//============================================================
#ifndef ZABSTRACTPOPULATION_H
#define ZABSTRACTPOPULATION_H
//============================================================
#include <QObject>
#include <QVariant>
//============================================================
class ZAbstractPopulation : public QObject
{
    Q_OBJECT
public:
    explicit ZAbstractPopulation(QObject* parent = nullptr);

    virtual void zp_generate(QVariant vSettings) = 0;
    virtual void zp_clear() = 0;

    virtual void zp_populationSize(quint64& size) const = 0;
    virtual void zp_populationHealthStatus(QMap<int, quint64>& populationHealthStatus) const = 0;
    virtual void zp_populationHealthStatusReport(
        QMap<QString, quint64>& populationHealthStatus) const = 0;
    virtual quint64 zp_numberForHealthStatus(int healthStatus) const = 0;

signals:
    void zg_populationStateChanged() const;
};
//============================================================
#endif // ZABSTRACTPOPULATION_H
