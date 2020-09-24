//============================================================
#include "ZStochasticHeterogeneousSIRProcessDashBoard.h"
#include "ZStochasticHeterogeneousSIRProcess.h"
#include <QLabel>
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

    return true;
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QLabel* label = new QLabel("POP");
    mainLayout->addWidget(label);
}
//============================================================
void ZStochasticHeterogeneousSIRProcessDashBoard::zh_createConnections() {}
//============================================================
