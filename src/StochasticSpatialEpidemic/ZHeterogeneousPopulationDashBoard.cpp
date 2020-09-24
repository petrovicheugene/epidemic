//============================================================
#include "ZHeterogeneousPopulationDashBoard.h"
#include "ZHeterogeneousPopulation.h"
#include <QLabel>
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

    return true;
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QLabel* label = new QLabel("POP");
    mainLayout->addWidget(label);
}
//============================================================
void ZHeterogeneousPopulationDashBoard::zh_createConnections() {}
//============================================================
