//============================================================
#ifndef ZEPIDEMICPROCESS_H
#define ZEPIDEMICPROCESS_H
//============================================================
#include <QObject>
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

    virtual void zp_start() = 0;
    virtual void zp_pause() = 0;
    virtual void zp_stop() = 0;

signals:

    void zg_groupsChanged(const QString& groupName, int size) const;
    void zg_finished() const;

private:
    // VARS

    // FUNCS
};
//============================================================
#endif // ZEPIDEMICPROCESS_H
