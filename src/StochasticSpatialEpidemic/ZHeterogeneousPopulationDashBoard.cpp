//============================================================
#include "ZHeterogeneousPopulationDashBoard.h"
#include "ZHeterogeneousPopulation.h"
#include "ZHeterogeneousPopulationWidget.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QGroupBox>

#include <QLabel>
#include <QMetaObject>
#include <QPushButton>
#include <QRectF>
#include <QSettings>
#include <QSlider>
#include <QSpinBox>
#include <QStackedWidget>
#include <QVBoxLayout>
//============================================================
ZHeterogeneousPopulationDashBoard::ZHeterogeneousPopulationDashBoard(QWidget* parent)
    : ZAbstractDashBoard(parent)
{
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//============================================================
ZHeterogeneousPopulationDashBoard::~ZHeterogeneousPopulationDashBoard()
{
    zh_saveSettings();
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(this->objectName());

    settings.setValue(zv_generationSizeSpinBox->objectName(), zv_generationSizeSpinBox->value());
    settings.setValue(zv_markSizeSlider->objectName(), zv_markSizeSlider->value());

    settings.setValue(zv_distributionComboBox->objectName(),
                      zv_distributionComboBox->currentIndex());
    settings.setValue(zv_plotLengthSpinBox->objectName(), zv_plotLengthSpinBox->value());
    settings.setValue(zv_plotWidthSpinBox->objectName(), zv_plotWidthSpinBox->value());
    settings.setValue(zv_centerXSpinBox->objectName(), zv_centerXSpinBox->value());
    settings.setValue(zv_centerYSpinBox->objectName(), zv_centerYSpinBox->value());
    settings.setValue(zv_dispersionSpinBox->objectName(), zv_dispersionSpinBox->value());

    while (!settings.group().isEmpty())
    {
        settings.endGroup();
    }
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_restoreSettings()
{
    QSettings settings;
    settings.beginGroup(this->objectName());

    zv_generationSizeSpinBox->setValue(
        settings.value(zv_generationSizeSpinBox->objectName()).toInt());
    zv_markSizeSlider->setValue(settings.value(zv_markSizeSlider->objectName()).toInt());
    zv_distributionComboBox->setCurrentIndex(
        settings.value(zv_distributionComboBox->objectName()).toInt());
    zv_plotLengthSpinBox->setValue(settings.value(zv_plotLengthSpinBox->objectName()).toDouble());
    zv_plotWidthSpinBox->setValue(settings.value(zv_plotWidthSpinBox->objectName()).toDouble());
    zv_centerXSpinBox->setValue(settings.value(zv_centerXSpinBox->objectName()).toDouble());
    zv_centerYSpinBox->setValue(settings.value(zv_centerYSpinBox->objectName()).toDouble());
    zv_dispersionSpinBox->setValue(settings.value(zv_dispersionSpinBox->objectName()).toDouble());

    while (!settings.group().isEmpty())
    {
        settings.endGroup();
    }
}
//============================================================
bool ZHeterogeneousPopulationDashBoard::zp_connect(QObject* component)
{
    ZHeterogeneousPopulation* population = qobject_cast<ZHeterogeneousPopulation*>(component);
    if (population)
    {
        connect(this,
                &ZHeterogeneousPopulationDashBoard::zg_invokeGeneration,
                population,
                &ZHeterogeneousPopulation::zp_generate);
        connect(this,
                &ZHeterogeneousPopulationDashBoard::zg_invokeClearPopulation,
                population,
                &ZHeterogeneousPopulation::zp_clear);

        return true;
    }

    ZHeterogeneousPopulationWidget* widget = qobject_cast<ZHeterogeneousPopulationWidget*>(
        component);
    if (widget)
    {
        connect(this,
                &ZHeterogeneousPopulationDashBoard::zg_plotRectChanged,
                widget,
                &ZHeterogeneousPopulationWidget::zp_setPlotRect);
        connect(this,
                &ZHeterogeneousPopulationDashBoard::zg_invokeSetMarkSize,
                widget,
                &ZHeterogeneousPopulationWidget::zp_setMarkSize);
        connect(this,
                &ZHeterogeneousPopulationDashBoard::zg_invokeFitPlotInView,
                widget,
                &ZHeterogeneousPopulationWidget::zp_fitInView);
        connect(widget,
                &ZHeterogeneousPopulationWidget::zg_positionMarked,
                this,
                &ZHeterogeneousPopulationDashBoard::zh_setDistributionCenter);

        // initial settings of population plot
        zh_onPlotRectChange(0.0);
        zh_initFitInView();
        zh_onMarkSizeChange(zv_markSizeSlider->value());
        return true;
    }

    return false;
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_createComponents()
{
    QVBoxLayout* bkgLayout = new QVBoxLayout;
    bkgLayout->setMargin(0);
    setLayout(bkgLayout);

    QGroupBox* populationGroupBox = new QGroupBox(tr("Population"));
    populationGroupBox->setObjectName("Population");
    populationGroupBox->setStyleSheet(
        "QGroupBox#Population::title {subcontrol-origin: margin; "
        "subcontrol-position: top center; "
        "padding: 5px 8000px 5px 8000px; background-color: #FF17365D; color: rgb(255, 255, 255);}");
    bkgLayout->addWidget(populationGroupBox);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    populationGroupBox->setLayout(mainLayout);
    mainLayout->addSpacing(20);

    // Population size
    QGroupBox* groupBox = new QGroupBox(tr("Generation size"));
    mainLayout->addWidget(groupBox);
    QVBoxLayout* groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);
    QHBoxLayout* hLayout = new QHBoxLayout;
    groupBoxLayout->addLayout(hLayout);

    zv_generationSizeSlider = new QSlider(Qt::Horizontal, this);
    zv_generationSizeSlider->setObjectName("generationSizeSlider");
    zv_generationSizeSlider->setRange(1, zv_maxGenerationSize);
    hLayout->addWidget(zv_generationSizeSlider);
    zv_generationSizeSpinBox = new QSpinBox(this);
    zv_generationSizeSpinBox->setObjectName("generationSizeSpinBox");
    zv_generationSizeSpinBox->setRange(1, zv_maxGenerationSize);
    hLayout->addWidget(zv_generationSizeSpinBox);

    // Mark size
    groupBox = new QGroupBox(tr("Mark size"));
    mainLayout->addWidget(groupBox);
    groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);
    hLayout = new QHBoxLayout;
    groupBoxLayout->addLayout(hLayout);

    zv_markSizeSlider = new QSlider(Qt::Horizontal, this);
    zv_markSizeSlider->setObjectName("markSizeSlider");
    zv_markSizeSlider->setRange(3, 50);
    hLayout->addWidget(zv_markSizeSlider);
    hLayout->addSpacing(zv_generationSizeSpinBox->sizeHint().width());

    // Distribution
    // type
    groupBox = new QGroupBox(tr("Distribution"));
    mainLayout->addWidget(groupBox);
    groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);
    hLayout = new QHBoxLayout;
    groupBoxLayout->addLayout(hLayout);
    // distribution type
    QLabel* label = new QLabel;
    label->setText(tr("Distribution type:"));
    hLayout->addWidget(label);
    zv_distributionComboBox = new QComboBox(this);
    zv_distributionComboBox->setObjectName("distributionComboBox");
    zv_distributionComboBox->addItems(ZGenerationSettings::zs_distributionNames());
    hLayout->addWidget(zv_distributionComboBox, zv_maxStretch);
    // distribution settings
    groupBoxLayout->addSpacing(15);
    zv_stackedWidget = new QStackedWidget;
    groupBoxLayout->addWidget(zv_stackedWidget);
    // uniform settings
    zv_stackedWidget->addWidget(zh_createUniformSettingsWidget());

    // normal settings
    zv_stackedWidget->addWidget(zh_createNormalSettingsWidget());

    // basement
    mainLayout->addStretch(zv_maxStretch);
    QDialogButtonBox* dialogButtonBox = new QDialogButtonBox;
    dialogButtonBox->setOrientation(Qt::Vertical);
    mainLayout->addWidget(dialogButtonBox);

    zv_fitInViewButton = new QPushButton("Fit in view");
    zv_fitInViewButton->setToolTip(tr("Fit the plot rect in the view"));
    dialogButtonBox->addButton(zv_fitInViewButton, QDialogButtonBox::ActionRole);
    zv_clearPopulationButton = new QPushButton("Clear");
    zv_clearPopulationButton->setToolTip(tr("Remove all individuals from the population"));
    dialogButtonBox->addButton(zv_clearPopulationButton, QDialogButtonBox::ActionRole);
    zv_generateButton = new QPushButton("Generate");
    zv_generateButton->setToolTip(tr("Generate individuals"));
    dialogButtonBox->addButton(zv_generateButton, QDialogButtonBox::ActionRole);
}
//============================================================
QWidget* ZHeterogeneousPopulationDashBoard::zh_createUniformSettingsWidget()
{
    QWidget* widget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    widget->setLayout(mainLayout);

    // plot length
    QLabel* label = new QLabel;
    label->setText(tr("Plot length:"));
    mainLayout->addWidget(label);
    QHBoxLayout* hLayout = new QHBoxLayout;
    mainLayout->addLayout(hLayout);
    zv_plotLengthSpinBox = new QDoubleSpinBox(this);
    zv_plotLengthSpinBox->setObjectName("plotLengthSpinBox");
    zv_plotLengthSpinBox->setRange(1, 99999);
    hLayout->addWidget(zv_plotLengthSpinBox);
    label->setBuddy(zv_plotLengthSpinBox);
    hLayout->addStretch(zv_maxStretch);
    // plot width
    label = new QLabel;
    label->setText(tr("Plot width:"));
    mainLayout->addWidget(label);
    hLayout = new QHBoxLayout;
    mainLayout->addLayout(hLayout);
    zv_plotWidthSpinBox = new QDoubleSpinBox(this);
    zv_plotWidthSpinBox->setObjectName("plotWidthSpinBox");
    zv_plotWidthSpinBox->setRange(1, 99999);
    hLayout->addWidget(zv_plotWidthSpinBox);
    label->setBuddy(zv_plotWidthSpinBox);
    hLayout->addStretch(zv_maxStretch);
    mainLayout->addStretch(zv_maxStretch);

    return widget;
}
//============================================================
QWidget* ZHeterogeneousPopulationDashBoard::zh_createNormalSettingsWidget()
{
    QWidget* widget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    widget->setLayout(mainLayout);

    QLabel* label = new QLabel;
    label->setText(tr("Distribution center:"));
    mainLayout->addWidget(label);
    QHBoxLayout* hLayout = new QHBoxLayout;
    mainLayout->addLayout(hLayout);

    label = new QLabel;
    label->setText(tr("X:"));
    hLayout->addWidget(label);
    zv_centerXSpinBox = new QDoubleSpinBox(this);
    zv_centerXSpinBox->setObjectName("centerXSpinBox");
    zv_centerXSpinBox->setRange(-99999, 99999);
    hLayout->addWidget(zv_centerXSpinBox);

    label = new QLabel;
    label->setText(tr("Y:"));
    hLayout->addWidget(label);
    zv_centerYSpinBox = new QDoubleSpinBox(this);
    zv_centerYSpinBox->setObjectName("centerYSpinBox");
    zv_centerYSpinBox->setRange(-99999, 99999);
    hLayout->addWidget(zv_centerYSpinBox);

    hLayout->addStretch(zv_maxStretch);
    label->setBuddy(zv_centerXSpinBox);

    label = new QLabel;
    label->setText(tr("Dispersion:"));
    mainLayout->addWidget(label);
    hLayout = new QHBoxLayout;
    mainLayout->addLayout(hLayout);
    zv_dispersionSpinBox = new QDoubleSpinBox(this);
    zv_dispersionSpinBox->setObjectName("dispersionSpinBox");
    zv_dispersionSpinBox->setRange(0, 99999);
    hLayout->addWidget(zv_dispersionSpinBox);
    hLayout->addStretch(zv_maxStretch);
    label->setBuddy(zv_dispersionSpinBox);

    mainLayout->addStretch(zv_maxStretch);

    return widget;
}

//============================================================
void ZHeterogeneousPopulationDashBoard::zh_createConnections()
{
    connect(zv_generationSizeSlider,
            &QSlider::valueChanged,
            zv_generationSizeSpinBox,
            &QSpinBox::setValue);
    connect(zv_generationSizeSpinBox,
            SIGNAL(valueChanged(int)),
            zv_generationSizeSlider,
            SLOT(setValue(int)));
    connect(zv_generationSizeSlider,
            &QSlider::valueChanged,
            this,
            &ZHeterogeneousPopulationDashBoard::zg_generationSizeChanged);
    connect(zv_generateButton,
            &QPushButton::clicked,
            this,
            &ZHeterogeneousPopulationDashBoard::zh_initGeneration);
    connect(zv_clearPopulationButton,
            &QPushButton::clicked,
            this,
            &ZHeterogeneousPopulationDashBoard::zh_initClearPopulation);
    connect(zv_fitInViewButton,
            &QPushButton::clicked,
            this,
            &ZHeterogeneousPopulationDashBoard::zh_initFitInView);

    connect(zv_distributionComboBox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(zh_onDistributionTypeChange(int)));
    connect(zv_plotLengthSpinBox,
            SIGNAL(valueChanged(qreal)),
            this,
            SLOT(zh_onPlotRectChange(qreal)));
    connect(zv_plotWidthSpinBox,
            SIGNAL(valueChanged(qreal)),
            this,
            SLOT(zh_onPlotRectChange(qreal)));

    connect(zv_markSizeSlider,
            &QSlider::valueChanged,
            this,
            &ZHeterogeneousPopulationDashBoard::zh_onMarkSizeChange);
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_initGeneration() const
{
    ZGenerationSettings settings = zh_createGenerationSettings();
    emit zg_invokeGeneration(QVariant::fromValue(settings));
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_onPlotRectChange(qreal)
{
    emit zg_plotRectChanged(
        QRectF(0.0, 0.0, zv_plotLengthSpinBox->value(), zv_plotWidthSpinBox->value()));
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_initClearPopulation() const
{
    emit zg_invokeClearPopulation();
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_initFitInView() const
{
    emit zg_invokeFitPlotInView();
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_setDistributionCenter(QPointF center)
{
    zv_centerXSpinBox->setValue(center.x());
    zv_centerYSpinBox->setValue(center.y());
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_onMarkSizeChange(int val) const
{
    emit zg_invokeSetMarkSize(val);
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_onDistributionTypeChange(int index)
{
    zv_stackedWidget->setCurrentIndex(index);
}
//============================================================
ZGenerationSettings ZHeterogeneousPopulationDashBoard::zh_createGenerationSettings() const
{
    ZGenerationSettings settings;
    settings.zp_setParameter(ZGenerationSettings::PN_GENERATION_SIZE,
                             zv_generationSizeSpinBox->value());

    settings.zp_setDistributionType(
        ZGenerationSettings::zs_distributionType(zv_distributionComboBox->currentText()));

    if (settings.zp_distributionType() == ZGenerationSettings::DT_UNIFORM)
    {
        int l = zv_plotLengthSpinBox->value();
        int w = zv_plotWidthSpinBox->value();
        QRectF rect(QPointF(0.0, 0.0), QSizeF(l, w));
        settings.zp_setParameter(ZGenerationSettings::PN_PLOT_RECT, QVariant(rect));
    }
    else if (settings.zp_distributionType() == ZGenerationSettings::DT_NORMAL)
    {
        settings.zp_setParameter(ZGenerationSettings::PN_DISPERSION_CENTER,
                                 QVariant(QPointF(zv_centerXSpinBox->value(),
                                                  zv_centerYSpinBox->value())));
        settings.zp_setParameter(ZGenerationSettings::PN_DISPERSION,
                                 QVariant(zv_dispersionSpinBox->value()));
    }

    return settings;
}
//============================================================
