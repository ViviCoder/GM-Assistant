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
#include <QSettings>

MainWindow::MainWindow(const QString &dir): QMainWindow(), sDir(dir), bModified(false), pAboutDial(new AboutDialog(this)), timer(new QTimer(this)), iTimerCount(0), smMapper(new QSignalMapper(this))
{
    setupUi(this);
    updateDisplay();

    timer->setInterval(100);
    timer->setSingleShot(false);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimer_timeout()));
    connect(treeMusic,SIGNAL(fileToPlay(std::string)),this,SLOT(playMusic(std::string)));
    connect(treeFX,SIGNAL(fileToPlay(std::string)),this,SLOT(playSound(std::string)));
    connect(smMapper,SIGNAL(mapped(int)),this,SLOT(loadRecent(int)));

    if (sDir!="")
    {
        sDir += '/';
    }

    // saving display settings
    QSettings settings;
    settings.beginGroup("mainWindow");
    resize(settings.value("size",size()).toSize());
    if (settings.value("maximized",false).toBool())
    {
        setWindowState(windowState() | Qt::WindowMaximized);
    }
    settings.endGroup();
    
    settings.beginGroup("files");
    slRecent = settings.value("recent").toStringList();
    // display the recently opened games in the menu
    actionR_ecent->setMenu(new QMenu);
    updateRecent("");
    sFileName = settings.value("last").toString();
    if (!sFileName.isEmpty())
    {
        // we load the game opened when GMA was closed
        on_action_Reload_triggered();
        setWindowTitle(QString("GM-Assistant - ")+QFileInfo(sFileName).fileName());
    }
    settings.endGroup();
}

MainWindow::~MainWindow()
{
    // saving display settings
    QSettings settings;
    settings.beginGroup("mainWindow");
    if (!isMaximized())
    {
        settings.setValue("size",size());
    }
    else
    {
        settings.setValue("size",sUnmaximizedSize);
    }
    settings.setValue("maximized",isMaximized());
    settings.endGroup();

    // updates the list of recently opened games
    settings.beginGroup("files");
    settings.setValue("last",sFileName);
    settings.setValue("recent",slRecent);
    settings.endGroup();
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
        if (!file.isEmpty())
        {
            try
            {
                eGame.fromFile(file.toStdString());
                updateDisplay();
                bModified = false;
                updateRecent(file);
                sFileName = file;
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
    if (sFileName.isEmpty())
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
    if (!file.isEmpty())
    {
        eGame.toFile(sFileName.toStdString());
//        action_Save->setEnabled(false);
        bModified = false;
        updateRecent(file);
        sFileName = file;
    }
}

void MainWindow::on_action_New_triggered()
{
/*    if (!bModified || (QMessageBox::question(this,QApplication::translate("action","Confirmation",0),QApplication::translate("action","The game has been modified since the last save. If you continue, these changes will be discarded. Are you sure you want to continue?",0),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes))
    {*/
        eGame.clear();
        updateDisplay();
        bModified = false;
        updateRecent("");
        sFileName = "";
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

void MainWindow::on_sliderMusic_sliderReleased()
{
    // new position in the music
    double position = (double)sliderMusic->value()/sliderMusic->maximum()*soundEngine.duration();
    double shift = position-double(iTimerCount)/TICK;
    if (shift>0)
    {
        soundEngine.move(position-double(iTimerCount)/TICK);
    }
    else
    {
        soundEngine.goTo(position);
    }
    // updating the timer count
    iTimerCount = floor(TICK*position);
}

void MainWindow::on_action_Reload_triggered()
{
    if (sFileName.isEmpty())
    {
        on_action_Load_triggered();
    }
    else
    {
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

void MainWindow::resizeEvent(QResizeEvent *e)
{
    sUnmaximizedSize = e->oldSize();
    QMainWindow::resizeEvent(e);
}

void MainWindow::updateRecent(const QString &fileName)
{
    // delete the new file if already present
    int index = slRecent.indexOf(fileName);
    if (index != -1)
    {
        slRecent.removeAt(index);
    }
    if (!sFileName.isEmpty())
    {
        // reorder the previous file if already present, add if not
        index = slRecent.indexOf(sFileName);
        if (index != -1)
        {
            slRecent.move(index,0);
        }
        else
        {
            slRecent.insert(0,sFileName);
            if (slRecent.size() > RECENT_NUMBER)
            {
                slRecent.removeLast();
            }
        }
    }
    // update the display
    QMenu *menu = actionR_ecent->menu();
    menu->clear();
    int i=1;
    QAction *action;
    for (QStringList::iterator it=slRecent.begin(); it != slRecent.end(); it++)
    {
        action = menu->addAction(QString("&%1 ").arg(i)+QFileInfo(*it).fileName());
        // mapping
        smMapper->setMapping(action,i);
        connect(action,SIGNAL(triggered()),smMapper,SLOT(map()));
        action->setStatusTip(*it);
        i++; 
    }
    // update the name of the file in the window title
    if (!fileName.isEmpty())
    {
        setWindowTitle(QString("GM-Assistant - ")+QFileInfo(fileName).fileName());
    }
    else
    {
        setWindowTitle(QString("GM-Assistant - ")+QApplication::translate("mainWindow","New game",0));
    }
}

void MainWindow::loadRecent(int index)
{
    QString file(slRecent[index-1]);
    updateRecent(file);
    sFileName = file;
    on_action_Reload_triggered();
}
