#include "MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(): QMainWindow()
{
    setupUi(this);
}

void MainWindow::on_action_Quit_triggered()
{
    close();
}

void MainWindow::on_action_Load_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,QApplication::translate("action_Load","Select the file to open",0),"",QApplication::translate("action_Load","XML files (*.xml)",0)); 
    eGame.fromFile(fileName.toStdString());
    textNotes->setText(eGame.notes().c_str());
}
