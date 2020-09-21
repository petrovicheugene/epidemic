//============================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//============================================================
#include <ZBaseMainWindow.h>
//============================================================
class MainWindow : public ZBaseMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    // VARS
    void zh_createComponents();
    void zh_createConnections();

    //FUNCS




};
//============================================================
#endif // MAINWINDOW_H
