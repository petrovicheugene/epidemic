//============================================================
#include "ZStochasticHeterogeneousProcessCommon.h"
//============================================================
QHash<HealthStatus, QString> healthStatusNames = initHealthStatusNames();
//============================================================
QHash<HealthStatus, QString> initHealthStatusNames()
{
    QHash<HealthStatus, QString> names;
    names.insert(HS_NOT_DEFINED, QObject::tr("Not defined"));
    names.insert(HS_SUSCEPTIBLE, QObject::tr("Susceptible"));
    names.insert(HS_INFECTIOUS, QObject::tr("Infectious"));
    names.insert(HS_RECOVERED, QObject::tr("Recovered"));
    return names;
}

//============================================================
QString healthStatusName(HealthStatus healthStatus)
{
    return healthStatusNames.value(healthStatus);
}
//============================================================
QColor healthStatusColor(HealthStatus healthStatus)
{
    switch (healthStatus)
    {
    case HS_NOT_DEFINED:
        return QColor(Qt::transparent);
    case HS_SUSCEPTIBLE:
        return QColor(Qt::blue);
    case HS_INFECTIOUS:
        return QColor(Qt::red);
    case HS_RECOVERED:
        return QColor(Qt::green);
    }

    return QColor();
}
//============================================================
QColor healthStatusColor(const QString& healthStatusName)
{
    return healthStatusColor(healthStatusForName(healthStatusName));
}
//============================================================
HealthStatus healthStatusForName(const QString& healthStatusName)
{
    return healthStatusNames.key(healthStatusName);
}
//============================================================
