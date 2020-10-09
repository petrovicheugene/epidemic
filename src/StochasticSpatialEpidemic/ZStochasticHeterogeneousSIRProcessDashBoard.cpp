//============================================================
#include "ZStochasticHeterogeneousSIRProcessDashBoard.h"

#include "ZRecoveryProbabilityCalculator.h"
#include "ZStochasticHeterogeneousSIRProcess.h"

#include <QDebug>
#include <QDialogButtonBox>
#include <QFontMetrics>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QtMath>
//============================================================
ZStochasticHeterogeneousSIRProcessDashBoard::ZStochasticHeterogeneousSIRProcessDashBoard(
    QWidget* parent)
    : ZAbstractDashBoard(parent)
{
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
    zh_applySettingsToProcess();
}
//============================================================
ZStochasticHeterogeneousSIRProcessDashBoard::~ZStochasticHeterogeneousSIRProcessDashBoard()
{
    zh_saveSettings();
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(this->metaObject()->className());

    settings.setValue(zv_infectionFactorSlider->objectName(), zv_infectionFactorSlider->value());
    settings.setValue(zv_recoveryDurationSlider->objectName(), zv_recoveryDurationSlider->value());
    settings.setValue(zv_recoveryStartingDaySlider->objectName(),
                      zv_recoveryStartingDaySlider->value());
    settings.setValue(zv_epidemicRateSlider->objectName(), zv_epidemicRateSlider->value());

    while (!settings.group().isEmpty())
    {
        settings.endGroup();
    }
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_restoreSettings()
{
    QSettings settings;
    settings.beginGroup(this->metaObject()->className());

    zv_infectionFactorSlider->setValue(
        settings.value(zv_infectionFactorSlider->objectName()).toInt());
    zv_recoveryDurationSlider->setValue(
        settings.value(zv_recoveryDurationSlider->objectName()).toInt());
    zv_recoveryStartingDaySlider->setValue(
        settings.value(zv_recoveryStartingDaySlider->objectName()).toInt());
    zv_epidemicRateSlider->setValue(settings.value(zv_epidemicRateSlider->objectName()).toInt());

    while (!settings.group().isEmpty())
    {
        settings.endGroup();
    }
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_applySettingsToProcess()
{
    zh_onRecoveryDurationSliderChange(zv_recoveryDurationSlider->value());
    zh_onRecoveryStartingDaySliderChange(zv_recoveryStartingDaySlider->value());
    zh_onSpeedSliderChange(zv_epidemicRateSlider->value());
    zh_onInfectionFactorSliderChange(zv_infectionFactorSlider->value());
}
//============================================================
bool ZStochasticHeterogeneousSIRProcessDashBoard::zp_connect(QObject* component)
{
    ZStochasticHeterogeneousSIRProcess* process = qobject_cast<ZStochasticHeterogeneousSIRProcess*>(
        component);
    if (!process)
    {
        return false;
    }

    connect(this,
            &ZStochasticHeterogeneousSIRProcessDashBoard::zg_epidemicCommand,
            process,
            &ZStochasticHeterogeneousSIRProcess::zp_dispatchCommand);

    connect(process,
            &ZStochasticHeterogeneousSIRProcess::zg_processChanged,
            this,
            &ZStochasticHeterogeneousSIRProcessDashBoard::zp_onProcessStatusChange);

    zv_epidemicRateSlider->valueChanged(zv_epidemicRateSlider->value());

    return true;
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_createComponents()
{
    QVBoxLayout* bkgLayout = new QVBoxLayout;
    bkgLayout->setMargin(0);
    setLayout(bkgLayout);

    QGroupBox* epidemicGroupBox = new QGroupBox(tr("Epidemic"));
    epidemicGroupBox->setObjectName("EpidemicGroupBox");
    epidemicGroupBox->setStyleSheet(
        "QGroupBox#EpidemicGroupBox::title {subcontrol-origin: margin; "
        "subcontrol-position: top center; "
        "padding: 5px 8000px 5px 8000px; background-color: #FF17365D; color: rgb(255, 255, 255);}");
    bkgLayout->addWidget(epidemicGroupBox);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    epidemicGroupBox->setLayout(mainLayout);
    mainLayout->addSpacing(20);

    QFontMetrics fm(QLineEdit().font());

    // infection
    QGroupBox* groupBox = new QGroupBox(tr("Infection"));
    mainLayout->addWidget(groupBox);
    QVBoxLayout* groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);

    QLabel* label = new QLabel(this);
    label->setText(tr("Infection characteristic distance:"));
    groupBoxLayout->addWidget(label);

    QHBoxLayout* hLayout = new QHBoxLayout;
    groupBoxLayout->addLayout(hLayout);
    zv_infectionFactorSlider = new QSlider(Qt::Horizontal, this);
    zv_infectionFactorSlider->setObjectName("InfectionFactorSlider");
    zv_infectionFactorSlider->setRange(3, 100);
    hLayout->addWidget(zv_infectionFactorSlider);
    zv_infectionFactorLineEdit = new QLineEdit(this);
    zv_infectionFactorLineEdit->setObjectName("InfectionFactorLineEdit");
    zv_infectionFactorLineEdit->setReadOnly(true);
    zv_infectionFactorLineEdit->setValidator(new QDoubleValidator);
    zv_infectionFactorLineEdit->setFixedWidth(fm.horizontalAdvance("00000"));
    hLayout->addWidget(zv_infectionFactorLineEdit);

    // recovery
    groupBox = new QGroupBox(tr("Recovery"));
    mainLayout->addWidget(groupBox);
    groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);

    // recovery rate

    label = new QLabel(this);
    label->setText(tr("Recovery duration in days:"));
    groupBoxLayout->addWidget(label);

    hLayout = new QHBoxLayout;
    groupBoxLayout->addLayout(hLayout);

    zv_recoveryDurationSlider = new QSlider(Qt::Horizontal, this);
    zv_recoveryDurationSlider->setObjectName("RecoveryDurationSlider");
    zv_recoveryDurationSlider->setRange(0, 50);
    hLayout->addWidget(zv_recoveryDurationSlider);
    zv_recoveryDurationLineEdit = new QLineEdit(this);
    zv_recoveryDurationLineEdit->setObjectName("RrecoveryDurationLineEdit");
    zv_recoveryDurationLineEdit->setReadOnly(true);
    zv_recoveryDurationLineEdit->setValidator(new QDoubleValidator);
    zv_recoveryDurationLineEdit->setFixedWidth(fm.horizontalAdvance("00000"));
    hLayout->addWidget(zv_recoveryDurationLineEdit);
    // recovery start
    label = new QLabel(this);
    label->setText(tr("Recovery starting day:"));
    groupBoxLayout->addWidget(label);

    hLayout = new QHBoxLayout;
    groupBoxLayout->addLayout(hLayout);
    zv_recoveryStartingDaySlider = new QSlider(Qt::Horizontal, this);
    zv_recoveryStartingDaySlider->setObjectName("RecoveryStartingDaySlider");
    zv_recoveryStartingDaySlider->setRange(1, 20);
    hLayout->addWidget(zv_recoveryStartingDaySlider);
    zv_recoveryStartingDayLineEdit = new QLineEdit(this);
    zv_recoveryStartingDayLineEdit->setReadOnly(false);
    zv_recoveryStartingDayLineEdit->setValidator(new QDoubleValidator);
    zv_recoveryStartingDayLineEdit->setFixedWidth(fm.horizontalAdvance("00000"));
    hLayout->addWidget(zv_recoveryStartingDayLineEdit);

    // epidemic rate
    groupBox = new QGroupBox(tr("Epidemic rate"));
    mainLayout->addWidget(groupBox);
    groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);

    zv_epidemicRateSlider = new QSlider(Qt::Horizontal, this);
    zv_epidemicRateSlider->setObjectName("EpidemicRateSlider");
    zv_epidemicRateSlider->setRange(0, 9);
    zv_epidemicRateSlider->setTickInterval(100);
    groupBoxLayout->addWidget(zv_epidemicRateSlider);

    // basement
    mainLayout->addStretch(zv_maxStretch);
    QDialogButtonBox* dialogButtonBox = new QDialogButtonBox;
    // dialogButtonBox->setOrientation(Qt::Vertical);
    mainLayout->addWidget(dialogButtonBox);

    zv_startPauseButton = new QPushButton("Start");
    zv_startPauseButton->setToolTip(tr("Start epidemic"));
    dialogButtonBox->addButton(zv_startPauseButton, QDialogButtonBox::ActionRole);
    zv_stopButton = new QPushButton("Stop");
    zv_stopButton->setToolTip(tr("Interrupt epidemic"));
    dialogButtonBox->addButton(zv_stopButton, QDialogButtonBox::ActionRole);
    zv_resetButton = new QPushButton("Reset");
    zv_resetButton->setToolTip(tr("Reset population health status"));
    dialogButtonBox->addButton(zv_resetButton, QDialogButtonBox::ActionRole);
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_createConnections()
{
    connect(zv_infectionFactorSlider,
            &QSlider::valueChanged,
            this,
            &ZStochasticHeterogeneousSIRProcessDashBoard::zh_onInfectionFactorSliderChange);
    connect(zv_recoveryDurationSlider,
            &QSlider::valueChanged,
            this,
            &ZStochasticHeterogeneousSIRProcessDashBoard::zh_onRecoveryDurationSliderChange);
    connect(zv_recoveryStartingDaySlider,
            &QSlider::valueChanged,
            this,
            &ZStochasticHeterogeneousSIRProcessDashBoard::zh_onRecoveryStartingDaySliderChange);

    connect(zv_epidemicRateSlider,
            &QSlider::valueChanged,
            this,
            &ZStochasticHeterogeneousSIRProcessDashBoard::zh_onSpeedSliderChange);
    connect(zv_startPauseButton,
            &QPushButton::clicked,
            this,
            &ZStochasticHeterogeneousSIRProcessDashBoard::zh_onStartButtonClick);

    connect(zv_stopButton,
            &QPushButton::clicked,
            this,
            &ZStochasticHeterogeneousSIRProcessDashBoard::zh_onStopButtonClick);

    connect(zv_resetButton,
            &QPushButton::clicked,
            this,
            &ZStochasticHeterogeneousSIRProcessDashBoard::zh_onClearButtonClick);
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_onInfectionFactorSliderChange(int value)
{
    qreal L = static_cast<qreal>(value) / 10.0;
    zv_infectionFactorLineEdit->setText(QString::number(L));
    emit zg_epidemicCommand(EC_SET_L_PARAMETER, QVariant(L));
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_onRecoveryDurationSliderChange(int value)
{
    qreal recoveryDurationFactor = qExp(-static_cast<qreal>(value) / 16.0) * 1.65;
    // cacl duration for display in LineEdit
    ZRecoveryProbabilityCalculator calculator;
    calculator.zp_setRecoveryDurationFactor(recoveryDurationFactor);
    calculator.zp_setStartingProbability(0.01);
    int canary = 0;
    int duration = 0;
    for (qreal probability = 0.0; probability < 0.95;)
    {
        if (probability > 0.05)
        {
            duration++;
        }
        probability = calculator.zp_calcProbability(probability);
        if (canary++ > 100)
        {
            break;
        }
    }

    zv_recoveryDurationLineEdit->setText(QString::number(duration));

    // set recoveryDurationFactor  to the calculator
    emit zg_epidemicCommand(EC_SET_RECOVERY_DURATION_FACTOR, QVariant(recoveryDurationFactor));
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_onRecoveryStartingDaySliderChange(int value)
{
    // set value to display
    zv_recoveryStartingDayLineEdit->setText(QString::number(value));
    // calc starting probability which gives the required starting day
    ZRecoveryProbabilityCalculator calculator;
    qreal probability;
    qreal startingProbability;
    for (startingProbability = 0.5; startingProbability /= 2.0;)
    {
        if (startingProbability < 0.000001)
        {
            break;
        }
        probability = startingProbability;
        calculator.zp_setStartingProbability(startingProbability);
        calculator.zp_setRecoveryDurationFactor(0.2);
        for (int i = 0; i < value; i++)
        {
            probability = calculator.zp_calcProbability(probability);
        }

        if (probability < 0.05)
        {
            break;
        }
    }

    // set starting prrobability to the calculator
    emit zg_epidemicCommand(EC_SET_STARTING_RECOVERY_PROBABILITY, QVariant(startingProbability));
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_onSpeedSliderChange(int value)
{
    emit zg_epidemicCommand(EC_SET_TIMER_INTERVAL, QVariant(1000 - value * 100));
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_onStartButtonClick()
{
    emit zg_epidemicCommand(EC_RUN);
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_onPauseButtonClick()
{
    emit zg_epidemicCommand(EC_PAUSE);
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_onStopButtonClick()
{
    emit zg_epidemicCommand(EC_STOP);
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_onClearButtonClick()
{
    emit zg_epidemicCommand(EC_RESET_HEALTH_STATUS);
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zp_onProcessStatusChange(int status, QVariant data)
{
    ProcessStatus processStatus = static_cast<ProcessStatus>(status);

    switch (processStatus)
    {
    case PS_STOPPED:
        zv_startPauseButton->setText(tr("Start"));
        zv_startPauseButton->setToolTip(tr("Start epidemic"));
        zv_startPauseButton->disconnect();
        connect(zv_startPauseButton,
                &QPushButton::clicked,
                this,
                &ZStochasticHeterogeneousSIRProcessDashBoard::zh_onStartButtonClick);

        break;
    case PS_RUNNING:
        zv_startPauseButton->setText(tr("Pause"));
        zv_startPauseButton->setToolTip(tr("Suspend epidemic running"));
        zv_startPauseButton->disconnect();
        connect(zv_startPauseButton,
                &QPushButton::clicked,
                this,
                &ZStochasticHeterogeneousSIRProcessDashBoard::zh_onPauseButtonClick);
        break;
    case PS_SUSPENDED:
        zv_startPauseButton->setText(tr("Resume"));
        zv_startPauseButton->setToolTip(tr("Resume epidemic running"));
        zv_startPauseButton->disconnect();
        connect(zv_startPauseButton,
                &QPushButton::clicked,
                this,
                &ZStochasticHeterogeneousSIRProcessDashBoard::zh_onStartButtonClick);
        break;
    }
}
//============================================================
