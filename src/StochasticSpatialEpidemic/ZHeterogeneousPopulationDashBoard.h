//============================================================
#ifndef ZHETEROGENEOUSPOPULATIONDASHBOARD_H
#define ZHETEROGENEOUSPOPULATIONDASHBOARD_H
//============================================================
#include <ZAbstractDashBoard.h>
//============================================================
class ZHeterogeneousPopulationDashBoard : public ZAbstractDashBoard
{
    Q_OBJECT
public:
    explicit ZHeterogeneousPopulationDashBoard(QWidget* parent = nullptr);
    bool zp_connect(QObject* component) override;

signals:

private:
    // VARS

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
};
//============================================================
#endif // ZHETEROGENEOUSPOPULATIONDASHBOARD_H
