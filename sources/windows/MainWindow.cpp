/*************************************************************************
* Copyright © 2011 Vincent Prat & Simon Nicolas
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*************************************************************************/

#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "QCustomTreeWidgetItem.h"
#include "ItemFactory.h"
#include <cmath>

MainWindow::MainWindow(const QString &dir): QMainWindow(), sDir(dir), bModified(false), pAboutDial(new AboutDialog(this)), timer(new QTimer(this)), iTimerCount(0)
{
    setupUi(this);
    updateDisplay();

    timer->setInterval(100);
    timer->setSingleShot(false);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimer_timeout()));
    connect(treeMusic,SIGNAL(fileToPlay(std::string)),this,SLOT(playMusic(std::string)));
    connect(treeFX,SIGNAL(fileToPlay(std::string)),this,SLOT(playSound(std::string)));

    if (sDir!="")
    {
        sDir += '/';
    }
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
/*    if (!bModified || (QMessageBox::question(this,QApplication::translate("action","Confirmation",0),QApplication::translate("action","The game has been modified since the last save. If you continue, these changes will be discarded. Are you sure you want to continue?",0),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes))
    {*/
        QString file = QFileDialog::getOpenFileName(this,QApplication::translate("action","Select the file to open",0),sDir+"examples",QApplication::translate("action","XML files (*.xml)",0)); 
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
//    }
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
//        action_Save->setEnabled(false);
        bModified = false;
    }
}

void MainWindow::on_actionS_ave_as_triggered()
{
    eGame.notes() = textNotes->toPlainText().toStdString();
    QString file = QFileDialog::getSaveFileName(this,QApplication::translate("action","Select the file to save",0),sDir+"examples",QApplication::translate("action","XML files (*.xml)",0));
    if (!file.isNull())
    {
        sFileName = file;
        eGame.toFile(sFileName.toStdString());
//        action_Save->setEnabled(false);
        bModified = false;
    }
}

void MainWindow::on_action_New_triggered()
{
/*    if (!bModified || (QMessageBox::question(this,QApplication::translate("action","Confirmation",0),QApplication::translate("action","The game has been modified since the last save. If you continue, these changes will be discarded. Are you sure you want to continue?",0),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes))
    {*/
        eGame.clear();
        updateDisplay();
        bModified = false;
//    }
}

void MainWindow::updateDisplay()
{
    treeScenario->setTree(&eGame.scenario());
    textNotes->setText(eGame.notes().c_str());
    treeHistory->setTree(&eGame.history());
    treeMusic->setTree(&eGame.music());
    treeFX->setTree(&eGame.effects());
    tableStats->setLists(&eGame.skills(),&eGame.characters());
    timer->stop();
    iTimerCount = 0;
    soundEngine.stop();
    sliderMusic->setValue(0);
    buttonMusic->setText(QApplication::translate("mainWindow","&Play",0));
}

void MainWindow::on_buttonMusic_clicked()
{
    if (soundEngine.isPlayingMusic())
    {
        if (soundEngine.isMusicPaused())
        {
            timer->start();
            soundEngine.resumeMusic();
            buttonMusic->setText(QApplication::translate("mainWindow","&Pause",0));
        }
        else
        {
            timer->stop();
            soundEngine.pauseMusic();
            buttonMusic->setText(QApplication::translate("mainWindow","&Resume",0));
        }
    }
    else
    {
        // we play the selected song (if it is a sound item)
        QTreeWidgetItem *qItem = treeMusic->currentItem();
        if (qItem != NULL)
        {
            Item *item = dynamic_cast<QCustomTreeWidgetItem*>(qItem)->branch()->item();
            if (item->type()==Item::tSound)
            {
                playMusic(dynamic_cast<SoundItem*>(item)->fileName());
            }
        }
        timer->start();
    }
}

void MainWindow::onTimer_timeout()
{
    if (!soundEngine.isPlayingMusic())
    {
        sliderMusic->setValue(sliderMusic->maximum());
        buttonMusic->setText(QApplication::translate("mainWindow","&Play",0));
        timer->stop();
    }
    // calculate the percentage of the music played
    iTimerCount++;
    if (!sliderMusic->isSliderDown())
    {
        // we move the slider only if the user is not moving it manually
        sliderMusic->setValue(floor(TICK*iTimerCount/soundEngine.duration()));
    }
}

void MainWindow::playMusic(const std::string &fileName)
{
    try
    {
        soundEngine.playMusic(fileName);
    }
    catch (std::runtime_error &e)
    {
        QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),e.what());
    }
    timer->start();
    buttonMusic->setText(QApplication::translate("mainWindow","&Pause",0));
    iTimerCount = 0;
}

void MainWindow::playSound(const std::string &fileName)
{
    try
    {
        soundEngine.playSound(fileName);
    }
    catch (std::runtime_error &e)
    {
        QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),e.what());
    }
}

void MainWindow::on_sliderMusic_released()
{
    // new position in the music
    double position = (double)sliderMusic->value()/sliderMusic->maximum()*soundEngine.duration();
    soundEngine.move(position);
    // updating the timer count
    iTimerCount = floor(TICK*position);
}

void MainWindow::on_sliderMusic_wheeled(bool positive)
{
    int value = sliderMusic->value();
    if (positive && value < sliderMusic->maximum())
    {
        sliderMusic->setValue(sliderMusic->value()+sliderMusic->pageStep());
    }
    else if (!positive && value > 0)
    {
        sliderMusic->setValue(sliderMusic->value()-sliderMusic->pageStep());
    }
    on_sliderMusic_released();
}
