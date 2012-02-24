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
#include <QStackedLayout>

MainWindow::MainWindow(): QMainWindow(), bModified(false), pAboutDial(new AboutDialog(this)), timer(new QTimer(this)), iTimerCount(0), smMapper(new QSignalMapper(this)), dDuration(0.0)
{
    setupUi(this);
    updateDisplay();

    timer->setInterval(100);
    timer->setSingleShot(false);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimer_timeout()));
    connect(treeMusic,SIGNAL(fileToPlay(std::string,double)),this,SLOT(playMusic(std::string,double)));
    connect(treeFX,SIGNAL(fileToPlay(std::string,double)),this,SLOT(playSound(std::string)));
    connect(smMapper,SIGNAL(mapped(int)),this,SLOT(loadRecent(int)));
    // setting widgets
    treeFX->setLimitedSize(true);

    // loading settings
    QSettings settings;

    settings.beginGroup("directories");
    QDir::setCurrent(settings.value("work",settings.value("install",QDir::current().path()+"/examples").toString()).toString());
    settings.endGroup();

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
    // saving settings
    QSettings settings;

    settings.beginGroup("directories");
    settings.setValue("work",QDir::current().path());
    settings.endGroup();

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

void MainWindow::clearLayout()
{
    // Removing all the Widget from the Layout.
    GridLayout->removeWidget(gbCharacter);
    GridLayout->removeWidget(gbHistory);
    GridLayout->removeWidget(gbNote);
    GridLayout->removeWidget(gbScenario);
    GridLayout->removeWidget(gbSound);
    GridLayout->removeWidget(gbMusic);

    // Hiding the Widgets to prevent random showing.
    gbCharacter->hide();
    gbHistory->hide();
    gbMusic->hide();
    gbNote->hide();
    gbScenario->hide();
    gbSound->hide();

    // Unchecking all actions
    actionFull->setChecked(false);
    actionMusic->setChecked(false);
    actionSimple->setChecked(false);
    actionNoMusic->setChecked(false);
    actionDesign->setChecked(false);
}

void MainWindow::on_actionMusic_triggered()
{
    clearLayout();
    GridLayout->addWidget(gbMusic,0,0);
    GridLayout->addWidget(gbSound,0,1);
    gbMusic->show();
    gbSound->show();
    eGame.setUserInterface(Engine::uiMusic);
    actionMusic->setChecked(true);
}

void MainWindow::on_actionFull_triggered()
{
    clearLayout();
    GridLayout->addWidget(gbScenario,0,0);
    GridLayout->addWidget(gbNote,0,1);
    GridLayout->addWidget(gbCharacter,0,2);
    GridLayout->addWidget(gbHistory,1,0);
    GridLayout->addWidget(gbMusic,1,1);
    GridLayout->addWidget(gbSound,1,2);
    gbScenario->show();
    gbNote->show();
    gbCharacter->show();
    gbHistory->show();
    gbMusic->show();
    gbSound->show();
    eGame.setUserInterface(Engine::uiFull);
    actionFull->setChecked(true);
}

void MainWindow::on_actionSimple_triggered()
{
    clearLayout();
    GridLayout->addWidget(gbScenario,0,0,0,1);
    GridLayout->addWidget(gbMusic,0,1);
    GridLayout->addWidget(gbSound,1,1);
    gbScenario->show();
    gbMusic->show();
    gbSound->show();
    eGame.setUserInterface(Engine::uiSimple);
    actionSimple->setChecked(true);
}

void MainWindow::on_actionDesign_triggered()
{
    clearLayout();
    GridLayout->addWidget(gbScenario,0,0,0,1);
    GridLayout->addWidget(gbCharacter,0,1);
    GridLayout->addWidget(gbNote,1,1);
    gbScenario->show();
    gbCharacter->show();
    gbNote->show();
    eGame.setUserInterface(Engine::uiDesign);
    actionDesign->setChecked(true);
}

void MainWindow::on_actionNoMusic_triggered()
{
    clearLayout();
    GridLayout->addWidget(gbScenario,0,0);
    GridLayout->addWidget(gbHistory,1,0);
    GridLayout->addWidget(gbCharacter,0,1);
    GridLayout->addWidget(gbNote,1,1);
    gbScenario->show();
    gbHistory->show();
    gbCharacter->show();
    gbNote->show();
    eGame.setUserInterface(Engine::uiNoMusic);
    actionNoMusic->setChecked(true);
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
/*    if (!bModified || (QMessageBox::question(this,QApplication::translate("mainWindow","Confirmation",0),QApplication::translate("mainWindow","The game has been modified since the last save. If you continue, these changes will be discarded. Are you sure you want to continue?",0),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes))
    {*/
        QString file = QFileDialog::getOpenFileName(this,QApplication::translate("mainWindow","Select the file to open",0),QDir::current().path(),QApplication::translate("mainWindow","GM-Assistant files (*.gma);;XML files (*.xml)",0)); 
        if (!file.isEmpty())
        {
            try
            {
                // changing current directory
                QDir::setCurrent(QFileInfo(file).dir().path());
                eGame.fromFile(file.toStdString());
                updateDisplay();
                bModified = false;
                updateRecent(file);
                sFileName = file;
            }
            catch (xmlpp::exception &xml)
            {
                QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),xml.what());
            }
            catch (std::invalid_argument &e)
            {
                QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),QString(e.what()) + "\nThe game cannot be loaded.");
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
        try
        {
            eGame.notes() = textNotes->toPlainText().toStdString();
            eGame.toFile(sFileName.toStdString());
    //        action_Save->setEnabled(false);
            bModified = false;
        }
        catch (xmlpp::exception &xml)
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),xml.what());
        }
    }
}

void MainWindow::on_actionS_ave_as_triggered()
{
    eGame.notes() = textNotes->toPlainText().toStdString();
    QString file = QFileDialog::getSaveFileName(this,QApplication::translate("mainWindow","Select the file to save",0),QDir::current().path(),QApplication::translate("mainWindow","GM-Assistant files (*.gma);;XML files (*.xml)",0));
    if (!file.isEmpty())
    {
        try
        {
            eGame.toFile(file.toStdString());
    //        action_Save->setEnabled(false);
            bModified = false;
            updateRecent(file);
            sFileName = file;
            QDir::setCurrent(QFileInfo(sFileName).dir().path());
        }
        catch (xmlpp::exception &xml)
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),xml.what());
        }
    }
}

void MainWindow::on_action_New_triggered()
{
/*    if (!bModified || (QMessageBox::question(this,QApplication::translate("mainWindow","Confirmation",0),QApplication::translate("mainWindow","The game has been modified since the last save. If you continue, these changes will be discarded. Are you sure you want to continue?",0),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes))
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
    switch (eGame.userInterface())
    {
        case Engine::uiFull:    on_actionFull_triggered();
                                break;
        case Engine::uiSimple:  on_actionSimple_triggered();
                                break;
        case Engine::uiMusic:   on_actionMusic_triggered();
                                break;
        case Engine::uiDesign:  on_actionDesign_triggered();
                                break;
        case Engine::uiNoMusic: on_actionNoMusic_triggered();
                                break;
    }
    timer->stop();
    iTimerCount = 0;
    soundEngine.stop();
    dDuration = 0.0;
    sliderMusic->setValue(0);
    checkRepeat->setChecked(false);
    labelPosition->setText(QApplication::translate("mainWindow","0:00/0:00",0));
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
                SoundItem *sItem = dynamic_cast<SoundItem*>(item);
                playMusic(sItem->fileName(),sItem->duration());
            }
        }
        timer->start();
    }
}

void MainWindow::onTimer_timeout()
{
    if (!soundEngine.isPlayingMusic())
    {
        if (checkRepeat->isChecked())
        {
            playMusic(sCurrentMusic,dDuration);
        }
        else
        {
            sliderMusic->setValue(sliderMusic->maximum());
            labelPosition->setText(QApplication::translate("mainWindow","0:00/0:00",0));
            buttonMusic->setText(QApplication::translate("mainWindow","&Play",0));
            timer->stop();
        }
    }
    // calculate the percentage of the music played
    iTimerCount++;
    if (!sliderMusic->isSliderDown())
    {
        // we move the slider only if the user is not moving it manually
        double dPosition = (double)iTimerCount/TICK;
        sliderMusic->setValue(floor(dPosition/dDuration*sliderMusic->maximum()));
    }
}

void MainWindow::playMusic(const std::string &fileName, double duration)
{
    if (duration > 0.0)
    {
        try
        {
            soundEngine.playMusic(fileName);
            sCurrentMusic = fileName;
            timer->start();
            buttonMusic->setText(QApplication::translate("mainWindow","&Pause",0));
            iTimerCount = 0;
            dDuration = duration;
        }
        catch (std::runtime_error &e)
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),e.what());
        }
    }
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
    double position = (double)sliderMusic->value()/sliderMusic->maximum()*dDuration;
    soundEngine.move(position);
    // updating the timer count
    iTimerCount = floor(TICK*position);
}

void MainWindow::on_sliderMusic_valueChanged(int value)
{
    double dPosition = (double)value/sliderMusic->maximum()*dDuration;
    int duration = floor(dDuration);
    int position = floor(dPosition);
    labelPosition->setText(QString("%1:%2/%3:%4").arg(position/60).arg(position%60,2,10,QChar('0')).arg(duration/60).arg(duration%60,2,10,QChar('0')));
}

void MainWindow::on_action_Reload_triggered()
{
    if (sFileName.isEmpty())
    {
        on_action_Load_triggered();
    }
    else
    {
        // changing current directory
        QDir::setCurrent(QFileInfo(sFileName).dir().path());
        try
        {
            eGame.fromFile(sFileName.toStdString());
            updateDisplay();
            bModified = false;
        }
        catch (xmlpp::exception &xml)
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),xml.what());
        }
        catch (std::invalid_argument &e)
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),QString(e.what()) + "\nThe game cannot be loaded.");
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
