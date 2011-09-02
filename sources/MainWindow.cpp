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
    QString file = QFileDialog::getOpenFileName(this,QApplication::translate("action_Load","Select the file to open",0),"",QApplication::translate("action_Load","XML files (*.xml)",0)); 
    if (!file.isNull())
    {
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
}

void MainWindow::on_action_Save_triggered()
{
    if (fileName.isNull())
    {
        on_actionS_ave_as_triggered();
    }
    else
    {
        eGame.notes() = textNotes->toPlainText().toStdString();
        eGame.toFile(fileName.toStdString());
        action_Save->setEnabled(false);
    }
}

void MainWindow::on_actionS_ave_as_triggered()
{
    eGame.notes() = textNotes->toPlainText().toStdString();
    QString file = QFileDialog::getSaveFileName(this,QApplication::translate("action_Save","Select the file to save",0),"",QApplication::translate("action_Load","XML files (*.xml)",0));
    if (!file.isNull())
    {
        eGame.toFile(fileName.toStdString());
        action_Save->setEnabled(false);
    }
}

void MainWindow::on_textNotes_textChanged()
{
    action_Save->setEnabled(true);
}
