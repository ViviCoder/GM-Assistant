#include "MainWindow.h"

MainWindow::MainWindow(): QMainWindow()
{
    setupUi(this);
}

void MainWindow::on_action_Quit_triggered()
{
    close();
}
