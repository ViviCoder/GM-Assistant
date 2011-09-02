#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>

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
    try
    {
        eGame.fromFile(fileName.toStdString());
        textNotes->setText(eGame.notes().c_str());
    }
    catch (std::string s)
    {
        QMessageBox::critical(0,QApplication::translate("action_Load","Error",0),s.c_str());
    }
}
