#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(): QMainWindow(),eGame("game.xml"), bModified(false), iFailure("data/images/failure.png"),iSuccess("data/images/check.png"),iProgress("data/images/uncheck.png")
{
    setupUi(this);
    updateDisplay();
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
    // common brushes
    QBrush none(QColor("white")),progress(QColor(0x00FFFF88)),failure(QColor(0x00FF8888)),success(QColor(0x0088FF88));

    // scenario
    treeScenario->clear();
    treeScenario->setColumnCount(2);
    Tree &tree = eGame.scenario();
    std::vector<QTreeWidgetItem*> items;
    QTreeWidgetItem* item,*prev=NULL;
    int depth=0,oldDepth;
    for (Tree::iterator it=tree.begin(); it != tree.end(); it++)
    {
        oldDepth = depth;
        depth = it.depth();
        if (depth==0)
        {
            item = new QTreeWidgetItem(treeScenario);
        }
        else if (depth==oldDepth)
        {
            item = new QTreeWidgetItem(items[depth-1],prev);
        }
        else
        {
            item = new QTreeWidgetItem(items[depth-1]);
        }
        item->setText(0,(*it)->content().c_str());
        switch ((*it)->state())
        {
            case    sProgress:  item->setIcon(1,iProgress); break;
            case    sFailure:   item->setIcon(1,iFailure); break;
            case    sSuccess:   item->setIcon(1,iSuccess); break;
            default:    break;
        }
        prev = item;
        if (items.size() > (unsigned int)(depth))
        {
            items[depth] = item;
        }
        else
        {
            items.push_back(item);
        }
    }
    treeScenario->resizeColumnToContents(0);
    treeScenario->resizeColumnToContents(1);
    // notes
    textNotes->setText(eGame.notes().c_str());
    // characters (to do)
    // history
    listHistory->clear();
    List &list = eGame.history();
    QListWidgetItem *listItem;
    for (List::iterator it=list.begin(); it != list.end(); it++)
    {
        listItem = new QListWidgetItem((*it)->content().c_str(),listHistory);
        switch ((*it)->state())
        {
            case    sNone:      listItem->setBackground(none); break;
            case    sProgress:  listItem->setBackground(progress); break;
            case    sFailure:   listItem->setBackground(failure); break;
            case    sSuccess:   listItem->setBackground(success); break;
        }
    }
    // music
    listMusic->clear();
    List &list2 = eGame.music();
    for (List::iterator it=list2.begin(); it != list2.end(); it++)
    {
        listItem = new QListWidgetItem((*it)->content().c_str(),listMusic);
        switch ((*it)->state())
        {
            case    sNone:      listItem->setBackground(none); break;
            case    sProgress:  listItem->setBackground(progress); break;
            case    sFailure:   listItem->setBackground(failure); break;
            case    sSuccess:   listItem->setBackground(success); break;
        }
    }
    // effects
    listFX->clear();
    List &list3 = eGame.effects();
    for (List::iterator it=list3.begin(); it != list3.end(); it++)
    {
        listItem = new QListWidgetItem((*it)->content().c_str(),listFX);
        switch ((*it)->state())
        {
            case    sNone:      listItem->setBackground(none); break;
            case    sProgress:  listItem->setBackground(progress); break;
            case    sFailure:   listItem->setBackground(failure); break;
            case    sSuccess:   listItem->setBackground(success); break;
        }
    }
}
