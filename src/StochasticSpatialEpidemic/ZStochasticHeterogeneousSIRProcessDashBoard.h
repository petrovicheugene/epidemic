//============================================================
#ifndef ZSTOCHASTICHETEROGENEOUSSIRPROCESSDASHBOARD_H
#define ZSTOCHASTICHETEROGENEOUSSIRPROCESSDASHBOARD_H
//============================================================
#include "ZStochasticHeterogeneousProcessCommon.h"

#include <ZAbstractDashBoard.h>
#include <QVariant>
//============================================================
class QLabel;
class QLineEdit;
class QPushButton;
class QSlider;
class QSpinBox;
//============================================================
class ZStochasticHeterogeneousSIRProcessDashBoard : public ZAbstractDashBoard
{
    Q_OBJECT
public:
    explicit ZStochasticHeterogeneousSIRProcessDashBoard(QWidget *parent = nullptr);
    ~ZStochasticHeterogeneousSIRProcessDashBoard();
    bool zp_connect(QObject* component) override;

signals:

    void zg_epidemicCommand(int, QVariant data = QVariant()) const;

private slots:

    void zh_onInfectionCharacteristicDistanceChange(int value);
    void zh_onRecoveryDurationSliderChange(int value);
    void zh_onRecoveryStartingDaySliderChange(int value);
    void zh_onSpeedSliderChange(int value);
    void zh_onStartButtonClick();
    void zh_onPauseButtonClick();
    void zh_onStopButtonClick();
    void zh_onClearButtonClick();
    void zp_onProcessStatusChange(int status, QVariant data);

private:
    // VARS
    const int zv_maxStretch = 0xFFFFFF;

    QSlider* zv_infectionCharacteristicDistanceSlider;
    QSlider* zv_recoveryDurationSlider;
    QSlider* zv_recoveryStartingDaySlider;
    QLineEdit* zv_infectionFactorLineEdit;
    QLineEdit* zv_recoveryDurationLineEdit;
    QLineEdit* zv_recoveryStartingDayLineEdit;

    QSlider* zv_epidemicRateSlider;

    QPushButton* zv_resetButton;
    QPushButton* zv_startPauseButton;
    QPushButton* zv_stopButton;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_applySettingsToProcess();
    void zh_saveSettings() const;
    void zh_restoreSettings();
};
//============================================================
#endif // ZSTOCHASTICHETEROGENEOUSSIRPROCESSDASHBOARD_H
