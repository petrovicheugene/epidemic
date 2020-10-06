//============================================================
#ifndef ZHETEROGENEOUSPOPULATIONDASHBOARD_H
#define ZHETEROGENEOUSPOPULATIONDASHBOARD_H
//============================================================
#include "ZAbstractDashBoard.h"
#include "ZGenerationSettings.h"
#include <QVariant>
//============================================================
class QComboBox;
class QDoubleSpinBox;
class QPushButton;
class QSlider;
class QSpinBox;
class QStackedWidget;
//============================================================
class ZHeterogeneousPopulationDashBoard : public ZAbstractDashBoard
{
    Q_OBJECT
public:
    explicit ZHeterogeneousPopulationDashBoard(QWidget* parent = nullptr);
    ~ZHeterogeneousPopulationDashBoard();
    bool zp_connect(QObject* component) override;

signals:

    void zg_invokeGeneration(QVariant vSettings) const;
    void zg_invokeClearPopulation() const;
    void zg_invokeFitPlotInView() const;
    void zg_generationSizeChanged(int size) const;
    void zg_plotRectChanged(QRectF rect) const;
    void zg_invokeSetMarkSize(int val) const;

private slots:

    void zh_initGeneration() const;
    void zh_onDistributionTypeChange(int index);
    void zh_onPlotRectChange(qreal);
    void zh_initClearPopulation() const;
    void zh_initFitInView() const;
    void zh_setDistributionCenter(QPointF center);
    void zh_onMarkSizeChange(int sizeValue) const;

private:
    // VARS
    const int zv_maxGenerationSize = 10000;
    const int zv_maxStretch = 0xFFFFFF;
    QStackedWidget* zv_stackedWidget;
    QSlider* zv_generationSizeSlider;
    QSpinBox* zv_generationSizeSpinBox;
    QSlider* zv_markSizeSlider;
    QDoubleSpinBox* zv_plotWidthSpinBox;
    QDoubleSpinBox* zv_plotLengthSpinBox;
    QDoubleSpinBox* zv_centerXSpinBox;
    QDoubleSpinBox* zv_centerYSpinBox;

    QDoubleSpinBox* zv_dispersionSpinBox;

    QComboBox* zv_distributionComboBox;
    QPushButton* zv_generateButton;
    QPushButton* zv_clearPopulationButton;
    QPushButton* zv_fitInViewButton;

    // FUNCS
    void zh_createComponents();
    QWidget* zh_createUniformSettingsWidget();
    QWidget* zh_createNormalSettingsWidget();
    void zh_createConnections();
    ZGenerationSettings zh_createGenerationSettings() const;

    void zh_saveSettings() const;
    void zh_restoreSettings();
};
//============================================================
#endif // ZHETEROGENEOUSPOPULATIONDASHBOARD_H
