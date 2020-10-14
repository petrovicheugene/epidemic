QT += core gui
QT += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


#PRO VARS
TARGET = epidemic
#Open MP
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS +=  -fopenmp
LIBS += -fopenmp
#Application version
RC_ICONS = "virus-2.ico"

VER_MAJ=1
VER_MIN=0
VER_PAT=0

QMAKE_TARGET_PRODUCT="Epidemic"
QMAKE_TARGET_DESCRIPTION="Modeling of the epidemic spread"
QMAKE_TARGET_COMPANY="Eugene Petrovich"
QMAKE_TARGET_COPYRIGHT="Copyright © $${QMAKE_TARGET_COMPANY}. 2020.  All rights reserved."
#COMPANY_URL=www.eugenepetrovich.com
#PRODUCT_URL=www.bunqer_product.com
AUTHOR="Eugene Petrovich"
#AUTHORS_URL = www.bunqer_author.com
AUTHORS_EMAIL="petrovich.eugene@gmail.com"
#-------------------------------------------------
# in common.pri will be defined VERSION, TARGET, DEBUG SETTINGS
#  global APP DEFINES
#-------------------------------------------------
include( ../../common.pri )
include( ../../app.pri )

#END

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ZAbstractDashBoard.cpp \
    ZAbstractEpidemicProcess.cpp \
    ZAbstractFactory.cpp \
    ZAbstractPopulation.cpp \
    ZAbstractPopulationWidget.cpp \
    ZDistance.cpp \
    ZDistanceRepository.cpp \
    ZEpidemicDynamicWidget.cpp \
    ZGenerationSettings.cpp \
    ZHeterogeneousPopulation.cpp \
    ZHeterogeneousPopulationDashBoard.cpp \
    ZHeterogeneousPopulationScene.cpp \
    ZHeterogeneousPopulationView.cpp \
    ZHeterogeneousPopulationWidget.cpp \
    ZInfectionProbabilityCalculator.cpp \
    ZPositionedIndividual.cpp \
    ZPositionedIndividualGraphicsItem.cpp \
    ZRandom2DPositionGenerator.cpp \
    ZRecoveryProbabilityCalculator.cpp \
    ZStochasticHeterogeneousProcessCommon.cpp \
    ZStochasticHeterogeneousSIRFactory.cpp \
    ZStochasticHeterogeneousSIRProcess.cpp \
    ZStochasticHeterogeneousSIRProcessDashBoard.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h \
    ZAbstractDashBoard.h \
    ZAbstractEpidemicProcess.h \
    ZAbstractFactory.h \
    ZAbstractPopulation.h \
    ZAbstractPopulationWidget.h \
    ZDistance.h \
    ZDistanceRepository.h \
    ZEpidemicDynamicWidget.h \
    ZGenerationSettings.h \
    ZHeterogeneousPopulation.h \
    ZHeterogeneousPopulationDashBoard.h \
    ZHeterogeneousPopulationScene.h \
    ZHeterogeneousPopulationView.h \
    ZHeterogeneousPopulationWidget.h \
    ZInfectionProbabilityCalculator.h \
    ZPositionedIndividual.h \
    ZPositionedIndividualGraphicsItem.h \
    ZRandom2DPositionGenerator.h \
    ZRecoveryProbabilityCalculator.h \
    ZStochasticHeterogeneousProcessCommon.h \
    ZStochasticHeterogeneousSIRFactory.h \
    ZStochasticHeterogeneousSIRProcess.h \
    ZStochasticHeterogeneousSIRProcessDashBoard.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
