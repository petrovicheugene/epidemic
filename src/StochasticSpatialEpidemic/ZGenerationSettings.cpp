//============================================================
#include "ZGenerationSettings.h"
#include <QObject>
//============================================================
// STATIC
QMap<ZGenerationSettings::DistributionType, QString> ZGenerationSettings::zv_distributionNames
    = ZGenerationSettings::zh_initDistributionNames();
QMap<ZGenerationSettings::DistributionType, QString>
ZGenerationSettings::zh_initDistributionNames()
{
    QMap<ZGenerationSettings::DistributionType, QString> map;
    map.insert(DT_UNIFORM, QObject::tr("Uniform"));
    map.insert(DT_NORMAL, QObject::tr("Normal"));
    return map;
}
// END STATIC
//============================================================
ZGenerationSettings::ZGenerationSettings()
{
    zv_distributionType = DT_NOT_DEFINED;
}
//============================================================
QString ZGenerationSettings::zs_distributionName(ZGenerationSettings::DistributionType type)
{
    return zv_distributionNames.value(type);
}
//============================================================
ZGenerationSettings::DistributionType ZGenerationSettings::zs_distributionType(QString name)
{
    return zv_distributionNames.key(name, DT_NOT_DEFINED);
}
//============================================================
QStringList ZGenerationSettings::zs_distributionNames()
{
    return zv_distributionNames.values();
}
//============================================================
ZGenerationSettings::DistributionType ZGenerationSettings::zp_distributionType() const
{
    return zv_distributionType;
}
//============================================================
void ZGenerationSettings::zp_setDistributionType(ZGenerationSettings::DistributionType type)
{
    zv_distributionType = type;
}
//============================================================
void ZGenerationSettings::zp_setParameter(ParameterName name, const QVariant& value)
{
    zv_parameterMap[name] = value;
}
//============================================================
QVariant ZGenerationSettings::zp_parameter(ParameterName name)
{
    return zv_parameterMap.value(name, QVariant());
}
//============================================================
