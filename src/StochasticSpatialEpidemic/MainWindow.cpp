//============================================================
#include "MainWindow.h"
//============================================================
MainWindow::MainWindow(QWidget* parent) : ZBaseMainWindow(parent)
{
    zh_createComponents();
    zh_createConnections();

    zh_restoreSettings();
}
//============================================================
MainWindow::~MainWindow()
{
    zh_saveSettings();
}
//============================================================
void MainWindow::zh_createComponents() {}
//============================================================
void MainWindow::zh_createConnections() {}
//============================================================
