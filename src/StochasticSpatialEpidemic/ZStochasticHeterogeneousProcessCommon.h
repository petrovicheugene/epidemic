//============================================================
#ifndef ZSTOCHASTICHETEROGENEOUSPROCESSCOMMON_H
#define ZSTOCHASTICHETEROGENEOUSPROCESSCOMMON_H
//============================================================
#include <QColor>
#include <QHash>
#include <QObject>
#include <QString>
//============================================================
enum ProcessStatus
{
    PS_STOPPED,
    PS_RUNNING,
    PS_SUSPENDED
};

enum EpidemicCommand
{
    EC_STOP,
    EC_RUN,
    EC_PAUSE,
    EC_RESET_HEALTH_STATUS,
    EC_SET_TIMER_INTERVAL,
    EC_SET_L_PARAMETER,
    EC_SET_RECOVERY_DURATION_FACTOR,
    EC_SET_STARTING_RECOVERY_PROBABILITY

};

enum HealthStatus
{
    HS_NOT_DEFINED,
    HS_SUSCEPTIBLE,
    HS_INFECTIOUS,
    HS_RECOVERED
};

QHash<HealthStatus, QString> initHealthStatusNames();
HealthStatus healthStatusForName(const QString& healthStatusName);
QString healthStatusName(HealthStatus healthStatus);
QColor healthStatusColor(HealthStatus healthStatus);
QColor healthStatusColor(const QString& healthStatusName);
//============================================================
#endif // ZSTOCHASTICHETEROGENEOUSPROCESSCOMMON_H
