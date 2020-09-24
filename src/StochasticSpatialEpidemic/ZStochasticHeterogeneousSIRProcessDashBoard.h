//============================================================
#ifndef ZSTOCHASTICHETEROGENEOUSSIRPROCESSDASHBOARD_H
#define ZSTOCHASTICHETEROGENEOUSSIRPROCESSDASHBOARD_H
//============================================================
#include <ZAbstractDashBoard.h>
//============================================================
class ZStochasticHeterogeneousSIRProcessDashBoard : public ZAbstractDashBoard
{
    Q_OBJECT
public:
    explicit ZStochasticHeterogeneousSIRProcessDashBoard(QWidget *parent = nullptr);
    bool zp_connect(QObject* component) override;

signals:

private:
    // VARS

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
};
//============================================================
#endif // ZSTOCHASTICHETEROGENEOUSSIRPROCESSDASHBOARD_H
