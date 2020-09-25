//============================================================
#ifndef ZHETEROGENEOUSPOPULATIONDASHBOARD_H
#define ZHETEROGENEOUSPOPULATIONDASHBOARD_H
//============================================================
#include <ZAbstractDashBoard.h>
//============================================================
class QPushButton;
//============================================================
class ZHeterogeneousPopulationDashBoard : public ZAbstractDashBoard
{
    Q_OBJECT
public:
    explicit ZHeterogeneousPopulationDashBoard(QWidget* parent = nullptr);
    bool zp_connect(QObject* component) override;

signals:

    void zg_invokeGeneration() const;

private slots:

    void zp_initGeneration() const;

private:
    // VARS
    QPushButton* zv_generateButton;
    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
};
//============================================================
#endif // ZHETEROGENEOUSPOPULATIONDASHBOARD_H
