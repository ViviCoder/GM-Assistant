#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "QCustomTreeWidgetItem.h"

MainWindow::MainWindow(): QMainWindow(),eGame("game.xml"), bModified(false), pAboutDial(new AboutDialog(this))
{
    setupUi(this);
    treeScenario->setTree(&eGame.scenario());
    treeHistory->setTree(&eGame.history());
    treeMusic->setTree(&eGame.music());
    treeMusic->setSoundEngine(&eGame.soundEngine(),true);
    treeFX->setTree(&eGame.effects());
    treeFX->setSoundEngine(&eGame.soundEngine());
    updateDisplay();
}

void MainWindow::on_actionAbout_triggered()
{
    pAboutDial->exec();
}


void MainWindow::on_action_Quit_triggered()
{
    close();
}

void MainWindow::on_action_Load_triggered()
{
    if (!bModified || (QMessageBox::question(this,QApplication::translate("action","Confirmation",0),QApplication::translate("action","The game has been modified since the last save. If you continue, these changes will be discarded. Are you sure you want to continue?",0),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes))
    {
        QString file = QFileDialog::getOpenFileName(this,QApplication::translate("action","Select the file to open",0),"",QApplication::translate("action","XML files (*.xml)",0)); 
        if (!file.isNull())
        {
            sFileName = file;
            try
            {
                eGame.fromFile(sFileName.toStdString());
                updateDisplay();
                bModified = false;
            }
            catch (xmlpp::exception &xml)
            {
                QMessageBox::critical(this,QApplication::translate("action","Error",0),xml.what());
            }
        }
    }
}

void MainWindow::on_action_Save_triggered()
{
    if (sFileName.isNull())
    {
        on_actionS_ave_as_triggered();
    }
    else
    {
        eGame.notes() = textNotes->toPlainText().toStdString();
        eGame.toFile(sFileName.toStdString());
        action_Save->setEnabled(false);
        bModified = false;
    }
}

void MainWindow::on_actionS_ave_as_triggered()
{
    eGame.notes() = textNotes->toPlainText().toStdString();
    QString file = QFileDialog::getSaveFileName(this,QApplication::translate("action","Select the file to save",0),"",QApplication::translate("action","XML files (*.xml)",0));
    if (!file.isNull())
    {
        sFileName = file;
        eGame.toFile(sFileName.toStdString());
        action_Save->setEnabled(false);
        bModified = false;
    }
}

void MainWindow::on_action_New_triggered()
{
    if (!bModified || (QMessageBox::question(this,QApplication::translate("action","Confirmation",0),QApplication::translate("action","The game has been modified since the last save. If you continue, these changes will be discarded. Are you sure you want to continue?",0),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes))
    {
        eGame.clear();
        updateDisplay();
        bModified = false;
    }
}

void MainWindow::on_textNotes_textChanged()
{
    bModified = true;
    action_Save->setEnabled(true);
}

void MainWindow::updateDisplay()
{
    treeScenario->setTree(&eGame.scenario());
    textNotes->setText(eGame.notes().c_str());
//    tableChar->setChars(***)
    treeHistory->setTree(&eGame.history());
    treeMusic->setTree(&eGame.music());
    treeFX->setTree(&eGame.effects());
}
