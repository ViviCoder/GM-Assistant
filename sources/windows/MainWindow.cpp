/*************************************************************************
* Copyright © 2011-2020 Vincent Prat & Simon Nicolas
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
#include <QTextStream>
#include <QLibraryInfo>
#include <QTranslator>
#include "MetadataModification.h"
#include <Poco/Exception.h>

MainWindow::MainWindow(const QString &install_dir): QMainWindow(), musicPlayer(new QMediaPlayer(this)), soundPlayer(new QMediaPlayer(this)), pAboutDial(new AboutDialog(this)), pDiceDialog(new DiceDialog(this)), pSelectCharacterDialog(new SelectCharacterDialog(this)), smRecent(new QSignalMapper(this)), siCurrentMusic(0), tApplication(new QTranslator(this)), tSystem(new QTranslator(this)), sInstall(install_dir), smLanguage(new QSignalMapper(this)), pMetadataDialog(new MetadataDialog(this)), pItemDialog(new ItemDialog(this))
{
    setupUi(this);
    updateDisplay();
    updateUndoRedo();

    // interface action group
    QActionGroup *interfaceGroup = new QActionGroup(this);
    interfaceGroup->addAction(actionMusic);
    interfaceGroup->addAction(actionFull);
    interfaceGroup->addAction(actionSimple);
    interfaceGroup->addAction(actionDesign);
    interfaceGroup->addAction(actionNoMusic);

    // language action group
    QActionGroup *languageGroup = new QActionGroup(this);

    // Listing available translations
    QFile file(sInstall + "translations/languages");
    QString locale = QLocale::system().name().section('_',0,0);
    bool locale_found = false;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QStringList parts = in.readLine().split(':');
            if (parts.size() == 2)
            {
                QAction *action = new QAction(this);
                action->setText(parts[0]);
                action->setCheckable(true);
                languageGroup->addAction(action);
                if (parts[1] == locale)
                {
                    action->setChecked(true);
                    locale_found = true;
                }
                else if (parts[1].isEmpty() && !locale_found)
                {
                    // default language: english
                    action->setChecked(true);
                }
                menu_Language->addAction(action);
                menu_Language->setEnabled(true);
                // signal mapping
                connect(action, SIGNAL(triggered()), smLanguage, SLOT(map()));
                smLanguage->setMapping(action, parts[1]);
            }
        }
        file.close();
        connect(smLanguage, SIGNAL(mapped(const QString&)), this, SLOT(translationRequested(const QString&)));
    }

    // Translators
    QApplication::installTranslator(tApplication);
    QApplication::installTranslator(tSystem);
    if (locale_found)
    {
        translationRequested(locale);
    }

    // connections
    connect(smRecent,SIGNAL(mapped(int)),this,SLOT(loadRecent(int)));
    connect(treePlot, SIGNAL(modificationDone(Modification*)), this, SLOT(registerModification(Modification*)));
    treePlot->installEventFilter(this);
    connect(treeHistory, SIGNAL(modificationDone(Modification*)), this, SLOT(registerModification(Modification*)));
    treeHistory->installEventFilter(this);
    connect(treeMusic, SIGNAL(modificationDone(Modification*)), this, SLOT(registerModification(Modification*)));
    treeMusic->installEventFilter(this);
    connect(treeFX, SIGNAL(modificationDone(Modification*)), this, SLOT(registerModification(Modification*)));
    treeFX->installEventFilter(this);
    connect(textNotes, SIGNAL(modificationDone(Modification*)), this, SLOT(registerModification(Modification*)));
    connect(textNotes, SIGNAL(unregistered()), this, SLOT(updateUndoRedo()));
    textNotes->installEventFilter(this);
    connect(tableStats, SIGNAL(modificationDone(Modification*)), this, SLOT(registerModification(Modification*)));
    tableStats->installEventFilter(this);

    // Item dialog
    treePlot->setItemDialogWindow(pItemDialog);
    treeHistory->setItemDialogWindow(pItemDialog);
    treeMusic->setItemDialogWindow(pItemDialog);
    treeFX->setItemDialogWindow(pItemDialog);

    // setting audio options
    treeMusic->setPlayingMethod(this, QCustomTreeWidget::pmMusic);
    treeFX->setPlayingMethod(this, QCustomTreeWidget::pmSound);
    connect(musicPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(onMusicPositionChanged(qint64)));
    connect(musicPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(onMusicDurationChanged(qint64)));
    connect(musicPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onMusicStateChanged(QMediaPlayer::State)));
    connect(musicPlayer, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayError(QMediaPlayer::Error)));

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
    actionR_ecent->setMenu(new QMenu(this));
    addRecent("");
    sFileName = settings.value("last").toString();
    if (!sFileName.isEmpty())
    {
        // we load the game opened when GMA was closed
        on_action_Reload_triggered();
        updateUndoRedo();
    }
    settings.endGroup();
    
    // menu updates
    connect(actionR_ecent->menu(),SIGNAL(aboutToShow()),this,SLOT(updateRecent()));

    // about Qt
    connect(actionAbout_Qt, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(aboutQt()));
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
    GridLayout->removeWidget(gbPlot);
    GridLayout->removeWidget(gbSound);
    GridLayout->removeWidget(gbMusic);

    // Hiding the Widgets to prevent random showing.
    gbCharacter->hide();
    gbHistory->hide();
    gbMusic->hide();
    gbNote->hide();
    gbPlot->hide();
    gbSound->hide();
}

void MainWindow::on_actionMusic_triggered()
{
    clearLayout();
    GridLayout->addWidget(gbMusic,0,0);
    GridLayout->addWidget(gbSound,0,1);
    gbMusic->show();
    gbSound->show();
    sGame.setUserInterface(Scenario::uiMusic);
    actionMusic->setChecked(true);
}

void MainWindow::on_actionFull_triggered()
{
    clearLayout();
    GridLayout->addWidget(gbPlot,0,0);
    GridLayout->addWidget(gbNote,0,1);
    GridLayout->addWidget(gbCharacter,0,2);
    GridLayout->addWidget(gbHistory,1,0);
    GridLayout->addWidget(gbMusic,1,1);
    GridLayout->addWidget(gbSound,1,2);
    gbPlot->show();
    gbNote->show();
    gbCharacter->show();
    gbHistory->show();
    gbMusic->show();
    gbSound->show();
    sGame.setUserInterface(Scenario::uiFull);
    actionFull->setChecked(true);
}

void MainWindow::on_actionSimple_triggered()
{
    clearLayout();
    GridLayout->addWidget(gbPlot,0,0,2,1);
    GridLayout->addWidget(gbMusic,0,1);
    GridLayout->addWidget(gbSound,1,1);
    gbPlot->show();
    gbMusic->show();
    gbSound->show();
    sGame.setUserInterface(Scenario::uiSimple);
    actionSimple->setChecked(true);
}

void MainWindow::on_actionDesign_triggered()
{
    clearLayout();
    GridLayout->addWidget(gbPlot,0,0,2,1);
    GridLayout->addWidget(gbCharacter,0,1);
    GridLayout->addWidget(gbNote,1,1);
    gbPlot->show();
    gbCharacter->show();
    gbNote->show();
    sGame.setUserInterface(Scenario::uiDesign);
    actionDesign->setChecked(true);
}

void MainWindow::on_actionNoMusic_triggered()
{
    clearLayout();
    GridLayout->addWidget(gbPlot,0,0);
    GridLayout->addWidget(gbHistory,1,0);
    GridLayout->addWidget(gbCharacter,0,1);
    GridLayout->addWidget(gbNote,1,1);
    gbPlot->show();
    gbHistory->show();
    gbCharacter->show();
    gbNote->show();
    sGame.setUserInterface(Scenario::uiNoMusic);
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
    if (canClose())
    {
        QString file = QFileDialog::getOpenFileName(this,QApplication::translate("mainWindow","Select the file to open",0),QDir::current().path(),QApplication::translate("mainWindow","GM-Assistant files (*.gms *.gma);;XML files (*.xml)",0),0,QFileDialog::DontUseNativeDialog);
        if (!file.isEmpty())
        {
            // changing current directory
            QDir::setCurrent(QFileInfo(file).dir().path());
            try
            {
                sGame.fromFile(file.toStdString());
            }
            catch (Poco::Exception &poco)
            {
                QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0), QString(poco.displayText().c_str()));
                sGame.clear();
                file = "";
            }
            catch (std::exception &e)
            {
                QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),QApplication::translate("mainWindow","The game cannot be loaded correctly for the following reason:",0) + "\n\n" + QString(e.what()) + "\n\n" + QApplication::translate("mainWindow","The game will be loaded anyway, but some features might not work properly.",0));
                sGame.fromFile(file.toStdString(), false);
            }
            updateDisplay();
            mqQueue.clear();
            addRecent(file);
            sFileName = file;
            updateUndoRedo();
            if (!sGame.configuration().isValid())
            {
                if (QMessageBox::warning(this, QApplication::translate("mainWindow", "Warning", 0), QApplication::translate("mainWindow", "The syntax of the game you have just loaded is not rigourously correct. Would you like to fix it now?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
                {
                    changeFormatIfNeeded();
                }
            }
        }
    }
}

void MainWindow::save(bool askForUpdate)
{
    if (sFileName.isEmpty())
    {
        on_actionS_ave_as_triggered();
    }
    else
    {
        try
        {
            if (askForUpdate && sGame.configuration().version() < Version() && QMessageBox::warning(this, QApplication::translate("mainWindow", "Warning", 0), QApplication::translate("mainWindow", "The game you want to save does not use the latest version of GM-Assistant files. Do you want to update it? If no, some features may not be saved properly."), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
            {
                changeFormatIfNeeded(false);
                return;
            }
            sGame.toFile(sFileName.toStdString());
            mqQueue.save();
            action_Save->setEnabled(false);
            updateUndoRedo();
        }
        catch (Poco::Exception &poco)
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0), QString(poco.displayText().c_str()));
        }
    }
}

void MainWindow::on_action_Save_triggered()
{
    save();
}

bool MainWindow::on_actionS_ave_as_triggered()
{
    QString filters(QApplication::translate("mainWindow","GM-Assistant files (1.2) (*.gms)",0));
    if (!sGame.configuration().isArchived())
    {
        filters += QApplication::translate("mainWindow",";;GM-Assistant files (1.1) (*.gma);;GM-Assistant files (1.0) (*.xml)",0);
    }
    QFileDialog *dial = new QFileDialog(this,QApplication::translate("mainWindow","Select the file to save",0),QDir::current().path(), filters);
    dial->setOption(QFileDialog::DontUseNativeDialog, true);
    dial->setAcceptMode(QFileDialog::AcceptSave);
    if (dial->exec() == QFileDialog::Accepted)
    {
        QString file = dial->selectedFiles()[0]; 
        // adding the suffix if not present
        QString sVersion = dial->selectedNameFilter().right(12);
        sVersion = sVersion.left(sVersion.indexOf(')'));
        Version version(sVersion.toStdString());
        QString suffix = dial->selectedNameFilter().right(5).left(4);
        if (!file.endsWith(suffix))
        {
            file.append(suffix);
        }
        try
        {
            sGame.setVersion(version);
            sGame.toFile(file.toStdString());
            mqQueue.save();
            action_Save->setEnabled(false);
            addRecent(file);
            sFileName = file;
            QDir::setCurrent(QFileInfo(sFileName).dir().path());
            updateUndoRedo();
        }
        catch (Poco::Exception &poco)
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),  QString(poco.displayText().c_str()));
            return false;
        }
    }
    else
    {
        return false;
    }
    delete dial;
    return true;
}

void MainWindow::on_action_New_triggered()
{
    if (canClose())
    {
        sGame.clear();
        sGame.setUserInterface(Scenario::uiFull);
        updateDisplay();
        mqQueue.clear();
        addRecent("");
        sFileName = "";
        updateUndoRedo();
    }
}

void MainWindow::updateDisplay()
{
    switch (sGame.userInterface())
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
    treePlot->setTree(&sGame.plot());
    textNotes->setNotes(&sGame.notes());
    treeHistory->setTree(&sGame.history());
    treeMusic->setTree(&sGame.music());
    treeFX->setTree(&sGame.effects());
    tableStats->setLists(&sGame.properties(),&sGame.characters());
    musicPlayer->stop();
    siCurrentMusic = 0;
    checkRepeat->setChecked(false);
    updateTimeDisplay();
}

void MainWindow::on_buttonMusic_clicked()
{
    switch (musicPlayer->state())
    {
        case QMediaPlayer::PlayingState:
            {
                musicPlayer->pause();
                buttonMusic->setText(QApplication::translate("mainWindow","&Resume",0));
                break;
            }
        case QMediaPlayer::PausedState:
            {
                musicPlayer->play();
                buttonMusic->setText(QApplication::translate("mainWindow","&Pause",0));
                break;
            }
        default:
            {
                // we play the selected song (if it is a sound item)
                QTreeWidgetItem *qItem = treeMusic->currentItem();
                if (qItem)
                {
                    Item *item = dynamic_cast<QCustomTreeWidgetItem*>(qItem)->branch()->item();
                    if (item->type()==Item::tSound)
                    {
                        SoundItem *sItem = dynamic_cast<SoundItem*>(item);
                        playMusic(sItem);
                    }
                }
            }
    }
}

void MainWindow::updateTimeDisplay()
{
    if (musicPlayer->state() == QMediaPlayer::StoppedState)
    {
        // resets the display
        labelPosition->setText("0:00/0:00");
        buttonMusic->setText(QApplication::translate("mainWindow","&Play",0));
        sliderMusic->setEnabled(false);
    }
    else
    {
        // update of the position display
        int position_s = musicPlayer->position() / 1000;
        int duration = musicPlayer->duration() / 1000;
        labelPosition->setText(QString("%1:%2/%3:%4").arg(position_s/60).arg(position_s%60,2,10,QChar('0')).arg(duration/60).arg(duration%60,2,10,QChar('0')));
    }
}

void MainWindow::playMusic(const SoundItem *item)
{
    if (item)
    {
        siCurrentMusic = item;
        musicPlayer->setMedia(QUrl::fromLocalFile(item->fileName().c_str()));
        musicPlayer->play();
    }
    else
    {
        QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0), QApplication::translate("mainWindow","Unable to play the file",0));
    }
}

void MainWindow::stopMusic(const SoundItem *item)
{
    if (item == siCurrentMusic)
    {
        siCurrentMusic = 0;
        musicPlayer->stop();
        updateTimeDisplay();
    }
}

void MainWindow::playSound(const SoundItem *item)
{
    if (item)
    {
        soundPlayer->setMedia(QUrl::fromLocalFile(item->fileName().c_str()));
        soundPlayer->play();
    }
}

void MainWindow::on_action_Reload_triggered()
{
    if (!canClose())
    {
        return;
    }
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
            sFileName = "";
            updateDisplay();
            return;
        }
        // changing current directory
        QDir::setCurrent(file.dir().path());
        try
        {
            sGame.fromFile(sFileName.toStdString());
        }
        catch (Poco::Exception &poco)
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0), QString(poco.displayText().c_str()));
            sGame.clear();
            sFileName = "";
        }
        catch (std::exception &e)
        {
            QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),QApplication::translate("mainWindow","The game cannot be loaded correctly for the following reason:",0) + "\n\n" + QString(e.what()) + "\n\n" + QApplication::translate("mainWindow","The game will be loaded anyway, but some features might not work properly.",0));
            sGame.fromFile(sFileName.toStdString(), false);
        }
        updateDisplay();
        mqQueue.clear();
        updateUndoRedo();
        if (!sGame.configuration().isValid())
        {
            if (QMessageBox::warning(this, QApplication::translate("mainWindow", "Warning", 0), QApplication::translate("mainWindow", "The syntax of the game you have just loaded is not rigourously correct. Would you like to fix it now?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
            {
                changeFormatIfNeeded();
            }
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    sUnmaximizedSize = e->oldSize();
    QMainWindow::resizeEvent(e);
}

void MainWindow::addRecent(const QString &fileName)
{
    if (fileName == sFileName)
    {
        return;
    }
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
    if (canClose())
    {
        QString file(slRecent[index-1]);
        addRecent(file);
        sFileName = file;
        // no more modification check needed
        mqQueue.save();
        on_action_Reload_triggered();
    }
}
    
void MainWindow::on_action_Undo_triggered()
{
    updateModification(mqQueue.undo(), true);
    updateUndoRedo();
}

void MainWindow::updateModification(Modification *modification, bool undo)
{
    if (!modification) return;
    switch (modification->type())
    {
        case Modification::tTree:   
            {
                TreeModification* treeModif = dynamic_cast<TreeModification*>(modification);
                Tree *adr = &treeModif->tree();
                if (adr == &sGame.plot())
                {
                    treePlot->updateModification(treeModif, undo);
                }
                else if (adr == &sGame.history())
                {
                    treeHistory->updateModification(treeModif, undo);
                }
                else if (adr == &sGame.music())
                {
                    treeMusic->updateModification(treeModif, undo);
                }
                else if (adr == &sGame.effects())
                {
                    treeFX->updateModification(treeModif, undo);
                }
                break;
            }
        case Modification::tNote:   textNotes->updateModification(dynamic_cast<NoteModification*>(modification), undo);
                                    break;
        case Modification::tCharacter:  tableStats->updateModification(dynamic_cast<CharacterModification*>(modification), undo);
                                        break;
        default : break;
    }
}

void MainWindow::on_action_Redo_triggered()
{
    updateModification(mqQueue.redo(), false);
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
    bool modified = !mqQueue.isUpToDate() || textNotes->unregisteredModification();
    action_Save->setEnabled(modified);

    QString windowTitle("GM-Assistant - ");
    QString title(sGame.metadata().title().c_str());
    QString fileName(QFileInfo(sFileName).absoluteFilePath());

    if (sFileName.isEmpty())
    {
        // if no file name is given, we put "New game" instead
        fileName = QApplication::translate("mainWindow", "New game", 0);
    }

    if (!title.isEmpty())
    {
        // if there is a title, we put the file name in parentheses
        windowTitle += title + " (" + fileName + ")";
    }
    else
    {
        // otherwise we just put the file name
        windowTitle += fileName;
    }

    if (modified)
    {
        windowTitle += "*";
    }
    setWindowTitle(windowTitle);
}

bool MainWindow::eventFilter(QObject *source, QEvent *e)
{
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *event = dynamic_cast<QKeyEvent*>(e);
        Qt::KeyboardModifiers modifiers = event->modifiers();
        switch (event->key())
        {
            case Qt::Key_Z: // undo - redo
                            if (modifiers == Qt::ControlModifier)
                            {
                                if (source == textNotes)
                                {
                                    textNotes->checkModification();
                                }
                                if (mqQueue.undoable())
                                {
                                    on_action_Undo_triggered();
                                }
                                return true;
                            }
                            else if (modifiers == (Qt::ControlModifier | Qt::ShiftModifier))
                            {
                                if (source == textNotes)
                                {
                                    textNotes->checkModification();
                                }
                                if (mqQueue.redoable())
                                {
                                    on_action_Redo_triggered();
                                }
                                return true;
                            }
                            break;
            case Qt::Key_V: if (source == textNotes && modifiers == Qt::ControlModifier)
                            {
                                textNotes->forcePaste();
                            }
                            break;
            case Qt::Key_X: if (source == textNotes && modifiers == Qt::ControlModifier)
                            {
                                textNotes->forceCut();
                            }
                            break;
            default:    if (source == textNotes && modifiers & Qt::ControlModifier)
                        {
                            textNotes->checkModification();
                        }
                        break;
        }
    }
    return QMainWindow::eventFilter(source, e);
}

bool MainWindow::canClose()
{
    textNotes->clearFocus();
    if (mqQueue.isUpToDate())
    {
        // no need for confirmation
        return true;
    }
    switch (QMessageBox::question(this, QApplication::translate("mainWindow", "Confirmation", 0), QApplication::translate("mainWindow", "The current game has been modified since the last save. If you continue, unsaved changes will be discarded.", 0), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel))
    {
        case QMessageBox::Save: save();
        case QMessageBox::Discard:  return true;
        default:    return false;
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (canClose())
    {
        QMainWindow::closeEvent(e);
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::on_action_Dice_triggered()
{
    pDiceDialog->show();
}

void MainWindow::on_action_Combat_triggered()
{
    pSelectCharacterDialog->show(sGame.characters());
}

void MainWindow::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        retranslateUi(this);
        updateUndoRedo();
    }
}

void MainWindow::translationRequested(const QString &suffix)
{
    tApplication->load(sInstall + "translations/gmassistant_" + suffix);
#ifdef _WIN32
    tSystem->load(sInstall + "translations/qtbase_" + suffix);
#else
    tSystem->load("qtbase_" + suffix, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
}

void MainWindow::displayError(QMediaPlayer::Error error)
{
    if (error != QMediaPlayer::NoError)
    {
        QMessageBox::critical(this, QApplication::translate("mainWindow", "Error", 0), musicPlayer->errorString());
    }
}

void MainWindow::on_action_Metadata_triggered()
{
    Metadata &metadata = sGame.metadata();
    Metadata oldMetadata(metadata);
    if (pMetadataDialog->exec(oldMetadata) == QDialog::Accepted)
    {
        Metadata newMetadata = pMetadataDialog->metadata();
        if (oldMetadata != newMetadata)
        {
            metadata = newMetadata;
            registerModification(new MetadataModification(metadata, newMetadata, oldMetadata));
        }
    }
}

void MainWindow::changeFormatIfNeeded(bool askForUpdate)
{
    IOConfig config = sGame.configuration();
    if (askForUpdate)
    {
        sGame.setVersion(config.version());
    }
    else
    {
        sGame.setVersion(Version());
    }
    if (config.isArchived() == sGame.configuration().isArchived())
    {
        save(askForUpdate);
    }
    else
    {
        // we need to choose another file
        if (!on_actionS_ave_as_triggered())
        {
            // cancels the configuration change
            sGame.setConfig(config);
        }
    }
}

void MainWindow::changeCurrentMusic(const SoundItem *oldItem, const SoundItem *newItem)
{
    if (oldItem == siCurrentMusic)
    {
        siCurrentMusic = newItem;
    }
}

void MainWindow::onMusicStateChanged(QMediaPlayer::State state)
{
    switch (state)
    {
        case QMediaPlayer::PlayingState:
            {
                sliderMusic->setEnabled(true);
                buttonMusic->setText(QApplication::translate("mainWindow","&Pause",0));
                break;
            }
        case QMediaPlayer::StoppedState:
            {
                if (checkRepeat->isChecked())
                {
                    playMusic(siCurrentMusic);
                }
                else
                {
                    updateTimeDisplay();
                }
                break;
            }
        default: break;
    }
}

void MainWindow::onMusicPositionChanged(qint64 position)
{
    sliderMusic->setValue(position);
    updateTimeDisplay();
}

void MainWindow::onMusicDurationChanged(qint64 duration)
{
    sliderMusic->setMaximum(duration);
    updateTimeDisplay();
}

void MainWindow::on_sliderMusic_sliderMoved(int position)
{
    musicPlayer->setPosition(position);
}
