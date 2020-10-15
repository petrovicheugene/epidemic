//============================================================
#ifndef ZGENERATIONSETTINGS_H
#define ZGENERATIONSETTINGS_H
//============================================================
#include <QMap>
#include <QMetaType>
#include <QString>
#include <QVariant>
//============================================================
class ZGenerationSettings
{
    Q_GADGET
public:
    enum DistributionType
    {
        DT_NOT_DEFINED,
        DT_UNIFORM,
        DT_NORMAL
    };
    Q_ENUM(DistributionType)

    enum ParameterName
    {
        PN_GENERATION_SIZE,
        PN_PLOT_RECT,
        PN_DISPERSION_CENTER,
        PN_DISPERSION
    };
    Q_ENUM(ParameterName)

    // FUNCS
    ZGenerationSettings();
    static QString zs_distributionName(ZGenerationSettings::DistributionType);
    static ZGenerationSettings::DistributionType zs_distributionType(QString name);
    static QStringList zs_distributionNames();

    DistributionType zp_distributionType() const;
    void zp_setDistributionType(DistributionType type);

    void zp_setParameter(ParameterName name, const QVariant& value);
    QVariant zp_parameter(ParameterName name);

private:
    // VARS
    static QMap<DistributionType, QString> zv_distributionNames;
    DistributionType zv_distributionType;
    QMap<ParameterName, QVariant> zv_parameterMap;

    // FUNCS
    static QMap<DistributionType, QString> zh_initDistributionNames();
};
Q_DECLARE_METATYPE(ZGenerationSettings)
//============================================================
#endif // ZGENERATIONSETTINGS_H
