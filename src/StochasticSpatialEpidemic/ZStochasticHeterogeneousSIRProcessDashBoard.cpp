//============================================================
#include "ZStochasticHeterogeneousSIRProcessDashBoard.h"
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
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>
//============================================================
ZStochasticHeterogeneousSIRProcessDashBoard::ZStochasticHeterogeneousSIRProcessDashBoard(
    QWidget* parent)
    : ZAbstractDashBoard(parent)
{
    zh_createComponents();
    zh_createConnections();
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

    QGroupBox* populationGroupBox = new QGroupBox(tr("Epidemic"));
    populationGroupBox->setObjectName("EpidemicGroupBox");
    populationGroupBox->setStyleSheet(
        "QGroupBox#EpidemicGroupBox::title {subcontrol-origin: margin; "
        "subcontrol-position: top center; "
        "padding: 5px 8000px 5px 8000px; background-color: #FF17365D; color: rgb(255, 255, 255);}");
    bkgLayout->addWidget(populationGroupBox);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    populationGroupBox->setLayout(mainLayout);
    mainLayout->addSpacing(20);

    // recovery
    QGroupBox* groupBox = new QGroupBox(tr("Recovery"));
    mainLayout->addWidget(groupBox);
    QVBoxLayout* groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);

    //    // recovery rate

    QLabel* label = new QLabel(this);
    label->setText(tr("Recovery duration:"));
    groupBoxLayout->addWidget(label);

    QHBoxLayout* hLayout = new QHBoxLayout;
    groupBoxLayout->addLayout(hLayout);

    QFontMetrics fm(QLineEdit().font());

    zv_recoveryDurationSlider = new QSlider(Qt::Horizontal, this);
    zv_recoveryDurationSlider->setRange(1, 20);
    hLayout->addWidget(zv_recoveryDurationSlider);
    zv_recoveryDurationLineEdit = new QLineEdit(this);
    zv_recoveryDurationLineEdit->setReadOnly(true);
    zv_recoveryDurationLineEdit->setValidator(new QDoubleValidator);
    zv_recoveryDurationLineEdit->setFixedWidth(fm.horizontalAdvance("00000"));
    hLayout->addWidget(zv_recoveryDurationLineEdit);
    // recovery start
    label = new QLabel(this);
    label->setText(tr("Recovery start:"));
    groupBoxLayout->addWidget(label);

    hLayout = new QHBoxLayout;
    groupBoxLayout->addLayout(hLayout);
    zv_recoveryStartingDaySlider = new QSlider(Qt::Horizontal, this);
    zv_recoveryDurationSlider->setRange(1, 20);
    hLayout->addWidget(zv_recoveryStartingDaySlider);
    zv_recoveryStartingDayLineEdit = new QLineEdit(this);
    zv_recoveryStartingDayLineEdit->setReadOnly(false);
    zv_recoveryStartingDayLineEdit->setValidator(new QDoubleValidator);
    zv_recoveryStartingDayLineEdit->setFixedWidth(fm.horizontalAdvance("00000"));
    hLayout->addWidget(zv_recoveryStartingDayLineEdit);

    groupBox = new QGroupBox(tr("Epidemic rate"));
    mainLayout->addWidget(groupBox);
    groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);

    zv_epidemicRateSlider = new QSlider(Qt::Horizontal, this);
    zv_epidemicRateSlider->setRange(0, 9);
    zv_epidemicRateSlider->setTickInterval(100);
    groupBoxLayout->addWidget(zv_epidemicRateSlider);

    // basement
    mainLayout->addStretch(zv_maxStretch);
    QDialogButtonBox* dialogButtonBox = new QDialogButtonBox;
    dialogButtonBox->setOrientation(Qt::Vertical);
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
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_onRecoveryDurationSliderChange(int value)
{
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_onRecoveryStartingDaySliderChange(int value)
{
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
