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
    enum PopulationOperation
    {
        PO_READY,
        PO_REMOVING,
        PO_GENERATING
    };
    Q_ENUM(PopulationOperation)

    explicit ZAbstractPopulation(QObject* parent = nullptr);

    virtual void zp_generate(QVariant vSettings) = 0;
    virtual void zp_clear() = 0;

    virtual void zp_populationSize(quint64& size) const = 0;
    virtual void zp_populationHealthStatus(QMap<int, quint64>& populationHealthStatus) const = 0;
    virtual void zp_populationHealthStatusReport(
        QMap<QString, quint64>& populationHealthStatus) const = 0;
    virtual quint64 zp_numberForHealthStatus(int healthStatus) const = 0;
    virtual void zp_notifyPopulationHealthStatus() const = 0;

signals:
    void zg_populationStateChangeNotification() const;
    void zg_populationStateChanged(QMap<QString, quint64> populationHealthStatus,
                                   bool lastInStep = false) const;
    void zg_populationOperation(int operation, QString msg = QString()) const;
};
//============================================================
#endif // ZABSTRACTPOPULATION_H
