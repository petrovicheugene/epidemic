//============================================================
#include "ZHeterogeneousPopulationDashBoard.h"
#include "ZHeterogeneousPopulation.h"
#include <QPushButton>
#include <QVBoxLayout>
//============================================================
ZHeterogeneousPopulationDashBoard::ZHeterogeneousPopulationDashBoard(QWidget* parent)
    : ZAbstractDashBoard(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//============================================================
bool ZHeterogeneousPopulationDashBoard::zp_connect(QObject* component)
{
    ZHeterogeneousPopulation* population = qobject_cast<ZHeterogeneousPopulation*>(component);
    if (!population)
    {
        return false;
    }

    connect(this,
            &ZHeterogeneousPopulationDashBoard::zg_invokeGeneration,
            population,
            &ZHeterogeneousPopulation::zp_generate);

    return true;
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    zv_generateButton = new QPushButton("Generate");
    mainLayout->addWidget(zv_generateButton);
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_createConnections()
{
    connect(zv_generateButton,
            &QPushButton::clicked,
            this,
            &ZHeterogeneousPopulationDashBoard::zp_initGeneration);
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zp_initGeneration() const
{
    emit zg_invokeGeneration();
}
//============================================================
