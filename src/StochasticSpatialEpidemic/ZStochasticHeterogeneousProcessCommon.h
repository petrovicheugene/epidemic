//============================================================
#ifndef ZSTOCHASTICHETEROGENEOUSPROCESSCOMMON_H
#define ZSTOCHASTICHETEROGENEOUSPROCESSCOMMON_H
//============================================================
#include <QObject>
#include <QString>
//============================================================
enum ProcessStatus
{
    PS_STOPPED = 1,
    PS_RUNNING = 2,
    PS_SUSPENDED = 3
};

enum EpidemicCommand
{
    EC_STOP = 1,
    EC_RUN = 2,
    EC_PAUSE = 3,
    EC_RESET_HEALTH_STATUS = 4,
    EC_SET_TIMER_INTERVAL = 5,
    EC_RECALC_RECOVERY_PROBABILITY = 6,
    EC_SET_L_PARAMETER = 7

};

enum HealthStatus
{
    HS_NOT_DEFINED = 0,
    HS_SUSCEPTIBLE = 1,
    HS_INFECTIOUS = 2,
    HS_RECOVERED = 3
};

QString healthStatusName(HealthStatus healthStatus);

//============================================================
#endif // ZSTOCHASTICHETEROGENEOUSPROCESSCOMMON_H