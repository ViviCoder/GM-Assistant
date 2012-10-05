/*************************************************************************
* Copyright © 2011-2012 Vincent Prat & Simon Nicolas
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
#include "NoteModification.h"

MainWindow::MainWindow(): QMainWindow(), bModified(false), pAboutDial(new AboutDialog(this)), timer(new QTimer(this)), iTimerCount(0), smRecent(new QSignalMapper(this)), pDuration(0)
{
    setupUi(this);
    updateDisplay();
    updateUndoRedo();

    timer->setInterval(1000/TICK);
    timer->setSingleShot(false);
    // connections
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimer_timeout()));
    connect(smRecent,SIGNAL(mapped(int)),this,SLOT(loadRecent(int)));
    connect(treeScenario, SIGNAL(modificationDone(Modification*)), this, SLOT(registerModification(Modification*)));
    connect(treeHistory, SIGNAL(modificationDone(Modification*)), this, SLOT(registerModification(Modification*)));
    connect(treeMusic, SIGNAL(modificationDone(Modification*)), this, SLOT(registerModification(Modification*)));
    connect(treeFX, SIGNAL(modificationDone(Modification*)), this, SLOT(registerModification(Modification*)));
    connect(textNotes, SIGNAL(modificationDone(Modification*)), this, SLOT(registerModification(Modification*)));
    textNotes->installEventFilter(this);
    connect(tableStats, SIGNAL(modificationDone(Modification*)), this, SLOT(registerModification(Modification*)));
    // setting audio options
    treeFX->setSizeLimited(true);
    treeMusic->setPlayingMethod(this, QCustomTreeWidget::pmMusic);
    treeFX->setPlayingMethod(this, QCustomTreeWidget::pmSound);

    // loading settings
    QSettings settings;

    settings.beginGroup("directories");
    QDir::setCurrent(settings.value("work").toString());
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
    addRecent("");
    sFileName = settings.value("last").toString();
    if (!sFileName.isEmpty())
    {
        // we load the game opened when GMA was closed
        on_action_Reload_triggered();
        setWindowTitle(QString("GM-Assistant - ")+QFileInfo(sFileName).fileName());
    }
    settings.endGroup();
    
    // menu updates
    connect(actionR_ecent->menu(),SIGNAL(aboutToShow()),this,SLOT(updateRecent()));
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
    eGame.setUserInterface(Scenario::uiMusic);
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
    eGame.setUserInterface(Scenario::uiFull);
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
    eGame.setUserInterface(Scenario::uiSimple);
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
    eGame.setUserInterface(Scenario::uiDesign);
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
    eGame.setUserInterface(Scenario::uiNoMusic);
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
            // changing current directory
            QDir::setCurrent(QFileInfo(file).dir().path());
            try
            {
                eGame.fromFile(file.toStdString());
            }
            catch (xmlpp::exception &xml)
            {
                QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),xml.what());
            }
            catch (std::exception &e)
            {
                QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),QApplication::translate("mainWindow","The game cannot be loaded correctly for the following reason: ",0) + "\n\n" + QString(e.what()) + "\n\n" + QApplication::translate("mainWindow","The game will be loaded anyway, but some features might not work properly.",0));
                eGame.fromFile(file.toStdString(), false);
            }
            updateDisplay();
            mqQueue.clear();
            updateUndoRedo();
            bModified = false;
            addRecent(file);
            sFileName = file;
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
    QFileDialog *dial = new QFileDialog(this,QApplication::translate("mainWindow","Select the file to save",0),QDir::current().path(),QApplication::translate("mainWindow","GM-Assistant files (*.gma);;XML files (*.xml)",0));
    dial->setAcceptMode(QFileDialog::AcceptSave);
    if (dial->exec() == QFileDialog::Accepted)
    {
        QString file = dial->selectedFiles()[0]; 
        // adding the suffix if not present
        QString suffix = dial->selectedNameFilter().right(5).left(4);
        if (!file.endsWith(suffix))
        {
            file.append(suffix);
        }
        try
        {
            eGame.toFile(file.toStdString());
    //        action_Save->setEnabled(false);
            bModified = false;
            addRecent(file);
            sFileName = file;
            QDir::setCurrent(QFileInfo(sFileName).dir().path());
        }
        catch (xmlpp::exception &xml)
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),xml.what());
        }
    }
    delete dial;
}

void MainWindow::on_action_New_triggered()
{
/*    if (!bModified || (QMessageBox::question(this,QApplication::translate("mainWindow","Confirmation",0),QApplication::translate("mainWindow","The game has been modified since the last save. If you continue, these changes will be discarded. Are you sure you want to continue?",0),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes))
    {*/
        eGame.clear();
        eGame.setUserInterface(Scenario::uiFull);
        updateDisplay();
        mqQueue.clear();
        updateUndoRedo();
        bModified = false;
        addRecent("");
        sFileName = "";
//    }
}

void MainWindow::updateDisplay()
{
    switch (eGame.userInterface())
    {
        case Scenario::uiFull:    on_actionFull_triggered();
                                break;
        case Scenario::uiSimple:  on_actionSimple_triggered();
                                break;
        case Scenario::uiMusic:   on_actionMusic_triggered();
                                break;
        case Scenario::uiDesign:  on_actionDesign_triggered();
                                break;
        case Scenario::uiNoMusic: on_actionNoMusic_triggered();
                                break;
    }
    treeScenario->setTree(&eGame.scenario());
    textNotes->setNotes(&eGame.notes());
    treeHistory->setTree(&eGame.history());
    treeMusic->setTree(&eGame.music());
    treeFX->setTree(&eGame.effects());
    tableStats->setLists(&eGame.skills(),&eGame.characters());
    timer->stop();
    iTimerCount = 0;
    soundEngine.stop();
    pDuration = 0;
    checkRepeat->setChecked(false);
    updateTimeDisplay();
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
        if (qItem)
        {
            Item *item = dynamic_cast<QCustomTreeWidgetItem*>(qItem)->branch()->item();
            if (item->type()==Item::tSound)
            {
                SoundItem *sItem = dynamic_cast<SoundItem*>(item);
                playMusic(sItem->fileName(),sItem->duration());
            }
            updateTimeDisplay();
            timer->start();
        }
    }
}

void MainWindow::onTimer_timeout()
{
    if (!soundEngine.isPlayingMusic())
    {
        if (pDuration && *pDuration > 0 && checkRepeat->isChecked())
        {
            playMusic(sCurrentMusic, pDuration);
        }
        else
        {
            timer->stop();
            buttonMusic->setText(QApplication::translate("mainWindow","&Play",0));
            sliderMusic->setEnabled(false);
        }
    }
    // calculate the percentage of the music played
    iTimerCount++;
    if (!sliderMusic->isSliderDown())
    {
        updateTimeDisplay();
    }
}

void MainWindow::updateTimeDisplay()
{
    if (pDuration)
    {
        // we move the slider only if the user is not moving it manually
        double dPosition = (double)iTimerCount/TICK;
        sliderMusic->setValue(floor(dPosition/(*pDuration)*sliderMusic->maximum()));
        // update of the position display
        int position = floor(dPosition);
        int duration = floor(*pDuration);
        labelPosition->setText(QString("%1:%2/%3:%4").arg(position/60).arg(position%60,2,10,QChar('0')).arg(duration/60).arg(duration%60,2,10,QChar('0')));
    }
    else
    {
        // resets the display
        sliderMusic->setValue(0);
        labelPosition->setText("0:00/0:00");
        buttonMusic->setText(QApplication::translate("mainWindow","&Play",0));
        sliderMusic->setEnabled(false);
    }
}

void MainWindow::playMusic(const std::string &fileName, const double *duration)
{
    if (duration && *duration > 0.0)
    {
        try
        {
            soundEngine.playMusic(fileName);
            sCurrentMusic = fileName;
            timer->start();
            sliderMusic->setEnabled(true);
            buttonMusic->setText(QApplication::translate("mainWindow","&Pause",0));
            iTimerCount = 0;
            pDuration = duration;
        }
        catch (std::runtime_error &e)
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),e.what());
        }
    }
    else
    {
        QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0), QApplication::translate("mainWindow","Unable to play the file",0));
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
    double position = (double)sliderMusic->value()/sliderMusic->maximum()*(*pDuration);
    soundEngine.move(position);
    // updating the timer count
    iTimerCount = floor(TICK*position);
    updateTimeDisplay();
}

void MainWindow::on_action_Reload_triggered()
{
    if (sFileName.isEmpty())
    {
        on_action_Load_triggered();
    }
    else
    {
        QFileInfo file(sFileName);
        // test of the existence
        if (!file.exists())
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),QApplication::translate("mainWindow","The file \"%1\" does not exist.",0).arg(sFileName));
            return;
        }
        // changing current directory
        QDir::setCurrent(file.dir().path());
        try
        {
            eGame.fromFile(sFileName.toStdString());
        }
        catch (xmlpp::exception &xml)
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),xml.what());
        }
        catch (std::exception &e)
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),QApplication::translate("mainWindow","The game cannot be loaded correctly for the following reason: ",0) + "\n\n" + QString(e.what()) + "\n\n" + QApplication::translate("mainWindow","The game will be loaded anyway, but some features might not work properly.",0));
            eGame.fromFile(sFileName.toStdString(), false);
        }
        updateDisplay();
        mqQueue.clear();
        updateUndoRedo();
        bModified = false;
    }
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    sUnmaximizedSize = e->oldSize();
    QMainWindow::resizeEvent(e);
}

void MainWindow::addRecent(const QString &fileName)
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
        }
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

void MainWindow::updateRecent()
{
    // update the display
    QMenu *menu = actionR_ecent->menu();
    menu->clear();
    int i=1;
    QAction *action;
    for (QStringList::iterator it=slRecent.begin(); it != slRecent.end(); it++)
    {
        QFileInfo file(*it);
        if (file.exists())
        {
            // file exists, so we add it into the menu
            action = menu->addAction(QString("&%1 ").arg(i)+file.fileName());
            // mapping
            smRecent->setMapping(action,i);
            connect(action,SIGNAL(triggered()),smRecent,SLOT(map()));
            action->setStatusTip(*it);
            i++; 
            if (i == RECENT_NUMBER)
            {
                // deletes all remaining files in the list
                slRecent.erase(it,slRecent.end());
                break;
            }
        }
        else
        {
            // mark the list item to be deleted
            (*it) = "";
        }
    }
    // delete empty items from the list
    slRecent = slRecent.filter(QRegExp(".+"));
}

void MainWindow::loadRecent(int index)
{
    QString file(slRecent[index-1]);
    addRecent(file);
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

void MainWindow::on_action_Undo_triggered()
{
    updateModification(mqQueue.undo());
    updateUndoRedo();
}

void MainWindow::updateModification(Modification *modification)
{
    if (!modification) return;
    switch (modification->type())
    {
        case Modification::tTree:   
            {
                TreeModification* treeModif = dynamic_cast<TreeModification*>(modification);
                Tree *adr = &treeModif->tree();
                if (adr == &eGame.scenario())
                {
                    treeScenario->updateDisplay();
                }
                else if (adr == &eGame.history())
                {
                    treeHistory->updateDisplay();
                }
                else if (adr == &eGame.music())
                {
                    treeMusic->updateDisplay();
                }
                else if (adr == &eGame.effects())
                {
                    treeFX->updateDisplay();
                }
                break;
            }
        case Modification::tNote:   textNotes->updateDisplay();
                                    break;
        case Modification::tCharacter:  tableStats->updateDisplay();
                                        break;
        default : break;
    }
}

void MainWindow::on_action_Redo_triggered()
{
    updateModification(mqQueue.redo());
    updateUndoRedo();
}

void MainWindow::registerModification(Modification *modification)
{
    mqQueue.add(modification);
    updateUndoRedo();
}

void MainWindow::updateUndoRedo()
{
    action_Undo->setEnabled(mqQueue.undoable());
    action_Redo->setEnabled(mqQueue.redoable());
}

bool MainWindow::eventFilter(QObject *source, QEvent *e)
{
    if (e->type() == QEvent::KeyRelease)
    {
        QKeyEvent *event = dynamic_cast<QKeyEvent*>(e);
        if (event->key() == Qt::Key_Z)
        {
            Qt::KeyboardModifiers modifiers = event->modifiers();
            if (modifiers == Qt::ControlModifier)
            {
                textNotes->checkModification();
                if (mqQueue.undoable())
                {
                    on_action_Undo_triggered();
                }
            }
            else if (modifiers == (Qt::ControlModifier | Qt::ShiftModifier))
            {
                textNotes->checkModification();
                if (mqQueue.redoable())
                {
                    on_action_Redo_triggered();
                }
            }
        }
    }
    return QMainWindow::eventFilter(source, e);
}
