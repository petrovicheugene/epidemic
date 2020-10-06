//============================================================
#include "ZStochasticHeterogeneousProcessCommon.h"
//============================================================
QString healthStatusName(HealthStatus healthStatus)
{
    switch (healthStatus)
    {
    case HS_NOT_DEFINED:
        return QObject::tr("Not defined");
    case HS_SUSCEPTIBLE:
        return QObject::tr("Susceptible");
    case HS_INFECTIOUS:
        return QObject::tr("Infectious");
    case HS_RECOVERED:
        return QObject::tr("Recovered");
    }

    return QString();
}
//============================================================
