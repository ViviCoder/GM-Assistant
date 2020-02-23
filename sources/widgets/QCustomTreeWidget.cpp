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

#include "QCustomTreeWidget.h"
#include "QCustomTreeWidgetItem.h"
#include "ItemFactory.h"
#include "ImageWindow.h"
#include <QApplication>
#include <QMessageBox>
#include <exception>
#include <QScrollBar>
#include <QFileDialog>
#include <sstream>

using namespace std;

QCustomTreeWidget::QCustomTreeWidget(QWidget *parent): QTreeWidget(parent), menuIcons(new QMenu(this)), pTree(0), pItemDial(), pDragSource(0), bNewlySelected(false), bEditing(false), pmMethod(pmNone)
{
    // creating actions
    actionNone = new QAction(this);
    actionNone->setIcon(QIcon(":/data/images/empty.svg"));
    actionProgress = new QAction(this);
    actionProgress->setIcon(QIcon(":/data/images/uncheck.svg"));
    actionFailure = new QAction(this);
    actionFailure->setIcon(QIcon(":/data/images/failure.svg"));
    actionSuccess = new QAction(this);
    actionSuccess->setIcon(QIcon(":/data/images/check.svg"));
    actionAdd = new QAction(this);
    actionAdd->setIcon(QIcon(":/data/images/add.svg"));
    actionDelete = new QAction(this);
    actionDelete->setIcon(QIcon(":/data/images/remove.svg"));
    actionEdit = new QAction(this);
    actionEdit->setIcon(QIcon(":/data/images/pencil.svg"));
    actionLaunch = new QAction(this);
    actionExport = new QAction(this);
    actionExport->setIcon(QIcon(":/data/images/son.svg"));
    // populating the pop-up menu
    menuIcons->addAction(actionNone);
    menuIcons->addAction(actionProgress);
    menuIcons->addAction(actionFailure);
    menuIcons->addAction(actionSuccess);
    menuIcons->addSeparator();
    menuIcons->addAction(actionAdd);
    menuIcons->addAction(actionDelete);
    menuIcons->addAction(actionEdit);
    menuIcons->addSeparator();
    menuIcons->addAction(actionLaunch);
    menuIcons->addAction(actionExport);
    // text of the actions
    retranslate();
    // connecting signals
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem *,int)), SLOT(on_itemChanged(QTreeWidgetItem*, int)));
    connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem *)), SLOT(onItemCollapsed(QTreeWidgetItem *)));
    connect(this, SIGNAL(itemExpanded(QTreeWidgetItem *)), SLOT(onItemExpanded(QTreeWidgetItem *)));
    connect(this, SIGNAL(itemSelectionChanged()), SLOT(on_itemSelectionChanged()));
}

QCustomTreeWidget::~QCustomTreeWidget()
{
    delete menuIcons;
}

void QCustomTreeWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    QTreeWidgetItem *qtwItem = itemAt(e->pos());
    if (qtwItem)
    {
        launchItem(qtwItem);
    }
}

void QCustomTreeWidget::launchItem(QTreeWidgetItem *qItem)
{
    Item *item = dynamic_cast<QCustomTreeWidgetItem*>(qItem)->branch()->item();
    switch (item->type())
    {
        case Item::tSound:  {
                                SoundItem *soundItem = dynamic_cast<SoundItem*>(item);
                                if (pmMethod == pmNone)
                                {
                                    QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0), QApplication::translate("customTree","Audio files can be played only in the Music and Sound effects modules.",0));
                                }
                                else
                                {
                                    // we send a signal to play the music (and do some other things)
                                    emit fileToPlay(soundItem);
                                }
                                break;
                            }
        case Item::tImage: {
                                ImageItem *imageItem = dynamic_cast<ImageItem*>(item);
                                ImageWindow *image = new ImageWindow(imageItem->fileName(),this);
                                if (image->error())
                                {
                                    QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0), QApplication::translate("customTree","Unable to display the file",0));
                                    delete image;
                                }
                                break;
                             }
        default:            break;
    }
}

void QCustomTreeWidget::mousePressEvent(QMouseEvent *e)
{
    QTreeWidgetItem *item = itemAt(e->pos());
    switch (e->button())
    {
        case Qt::LeftButton:    if (item)
                                {
                                    pDragSource = item;
                                    bNewlySelected = !item->isSelected();
                                    // the item will be selected (and not be unselected by mouseReleaseEvent)
                                }
                                QTreeWidget::mousePressEvent(e);
                                break;
        case Qt::RightButton:   if (item)
                                {
                                    setCurrentItem(item);
                                    QCustomTreeWidgetItem *qItem = dynamic_cast<QCustomTreeWidgetItem*>(item);
                                    Item *treeItem = qItem->branch()->item();
                                    Item::Type type = treeItem->type();
                                    switch (type)
                                    {
                                        case Item::tSound:  if (pmMethod == pmNone)
                                                            {
                                                                actionLaunch->setVisible(false);
                                                            }
                                                            else
                                                            {
                                                                actionLaunch->setIcon(QIcon(":/data/images/speaker.svg"));
                                                                actionLaunch->setStatusTip(QApplication::translate("customTree","Play the audio file",0));
                                                                actionLaunch->setText(QApplication::translate("customTree","P&lay",0));
                                                                actionLaunch->setVisible(true);
                                                            }
                                                            break;
                                        case Item::tImage:      actionLaunch->setIcon(QIcon(":/data/images/image.svg"));
                                                                actionLaunch->setStatusTip(QApplication::translate("customTree","Display the image",0));
                                                                actionLaunch->setText(QApplication::translate("customTree","Disp&lay",0));
                                                                actionLaunch->setVisible(true);
                                                                break;
                                        default:    actionLaunch->setVisible(false);
                                                    break;
                                    }
                                    // export
                                    actionExport->setVisible(Item::is(type, Item::tFile) && dynamic_cast<FileItem*>(treeItem)->isIncluded());
                                    // execute menu
                                    QAction* action = menuIcons->exec(e->globalPos());
                                    if (action == actionNone)
                                    {
                                        changeState(qItem, Item::sNone);
                                    }
                                    else if (action == actionProgress)
                                    {
                                        changeState(qItem, Item::sProgress);
                                    }
                                    else if (action == actionFailure)
                                    {
                                        changeState(qItem, Item::sFailure);
                                    }
                                    else if (action == actionSuccess)
                                    {
                                        changeState(qItem, Item::sSuccess);
                                    }
                                    else if (action == actionDelete)
                                    {
                                        deleteItem(item);
                                    }
                                    else if (action == actionAdd)
                                    {
                                        addItem(qItem);
                                    }
                                    else if (action == actionEdit)
                                    {
                                        addItem(qItem, true);
                                    }
                                    else if (action == actionLaunch)
                                    {
                                        launchItem(item);
                                    }
                                    else if (action == actionExport)
                                    {
                                        exportFile(treeItem);
                                    }
                                }
                                else
                                {
                                    addItem(0);
                                }
                                break;
        default:    break;
    }
}

void QCustomTreeWidget::mouseReleaseEvent(QMouseEvent *e)
{
    pDragSource = 0;
    QTreeWidgetItem *item = itemAt(e->pos());
    if (!bNewlySelected)
    {
        setCurrentItem(0);
    }
    else
    {
        setCurrentItem(item);
    }
    QTreeWidget::mouseReleaseEvent(e);
}

void QCustomTreeWidget::keyReleaseEvent(QKeyEvent *e)
{
    QTreeWidgetItem *qItem = currentItem();
    if (qItem)
    {
        scrollTo(qItem);
        QCustomTreeWidgetItem *customItem = dynamic_cast<QCustomTreeWidgetItem*>(qItem);
        switch (e->key())
        {
            case Qt::Key_F2:    if (!bEditing)
                                {
                                    switch (e->modifiers())
                                    {
                                        case Qt::ControlModifier:   addItem(customItem, true);
                                                                    break;
                                        case Qt::NoModifier:    qItem->setFlags(qItem->flags() | Qt::ItemIsEditable);
                                                                editItem(qItem);
                                                                bEditing = true;
                                                                qItem->setFlags(qItem->flags() & ~Qt::ItemIsEditable);
                                                                break;
                                        default:    QTreeWidget::keyReleaseEvent(e); break;
                                    }
                                }
                                break;
            case Qt::Key_Delete:    if (!bEditing)
                                    {
                                        deleteItem(qItem);
                                    }
                                    break;
            case Qt::Key_Insert:    if (!bEditing)
                                    {
                                        addItem(customItem);
                                    }
                                    break;
            case Qt::Key_Space: if (!bEditing)
                                {
                                    if (e->modifiers() == Qt::ControlModifier)
                                    {
                                        exportFile(customItem->branch()->item());
                                    }
                                    else
                                    {
                                        launchItem(qItem);
                                    }
                                }
                                break;
            case Qt::Key_Return:
            case Qt::Key_Enter:
            case Qt::Key_Escape:    bEditing = false;
                                    break;
            case Qt::Key_F5:    changeState(customItem, Item::sNone);
                                break;
            case Qt::Key_F6:    changeState(customItem, Item::sProgress);
                                break;
            case Qt::Key_F7:    changeState(customItem, Item::sFailure);
                                break;
            case Qt::Key_F8:    changeState(customItem, Item::sSuccess);
                                break;
            default:    QTreeWidget::keyReleaseEvent(e); break; 
        }
    }
}

void QCustomTreeWidget::on_itemChanged(QTreeWidgetItem* item, int column)
{
    if (pTree && item && column == 0)
    {
        Branch *branch = dynamic_cast<QCustomTreeWidgetItem*>(item)->branch();
        if (bEditing && branch->item()->content() != item->text(0).toStdString())
        {
            string content = branch->item()->content();
            emit modificationDone(new TreeModification(*pTree, content, item->text(0).toStdString(), pTree->indicesOf(branch)));
            branch->item()->setContent(item->text(0).toStdString());
        }
        resizeColumnToContents(0);
    }
}

void QCustomTreeWidget::setTree(Tree *tree)
{
    pTree = tree;
    updateDisplay();
}

void QCustomTreeWidget::updateDisplay(const string &indices)
{
    clear();
    setColumnCount(2);
    if (pTree)
    {
        // storing the scroll state
        QScrollBar *vbar = verticalScrollBar();
        int h = vbar->value();
        // iterating over the tree to populate the widget
        vector<QCustomTreeWidgetItem*> items;
        QCustomTreeWidgetItem* item, *focusItem=0;
        int depth=0;        
        for (Tree::iterator it = pTree->begin(); it != pTree->end(); it++)
        {
            depth = it.depth();
            if (depth==0)
            {
                item = new QCustomTreeWidgetItem(this, it.branch());
            }
            else
            {
                item = new QCustomTreeWidgetItem(items[depth-1], it.branch());
            }
            if (items.size() > (unsigned int)(depth))
            {
                items[depth] = item;
            }
            else
            {
                items.push_back(item);
            }
            // expand the item if needed
            if (it.branch()->item()->expanded())
            {
                expandItem(item);
            }
            if (!focusItem && indices == it.indices())
            {
                focusItem = item;
            }
        }
        resizeColumnToContents(0);
        resizeColumnToContents(1);
        // restoring the scroll state
        if (indices != "")
        {
            vbar->setValue(h);
        }
        if (focusItem)
        {
            scrollTo(focusItem);
            setCurrentItem(focusItem);
        }
    }
}

void QCustomTreeWidget::deleteItem(QTreeWidgetItem *item)
{
    Branch *branch = dynamic_cast<QCustomTreeWidgetItem*>(item)->branch();
    // stop the music if the item is a SoundItem
    if (pmMethod == pmMusic)
    {
        stopMusic(branch);
    }
    // delete item
    if (pTree)
    {
        string indices = pTree->indicesOf(branch);
        emit modificationDone(new TreeModification(*pTree, branch, indices));
        pTree->remove(indices, false);
    }
    // delete widgetItem
    delete item;
    resizeColumnToContents(0);
}

QIcon QCustomTreeWidget::icon(Item::State state)
{
    switch (state)
    {
        case Item::sProgress:   return QIcon(":/data/images/uncheck.svg");  break;
        case Item::sFailure:    return QIcon(":/data/images/failure.svg");  break;
        case Item::sSuccess:    return QIcon(":/data/images/check.svg");    break;
        default:                return QIcon(":/data/images/empty.svg");     break;
    }
}

void QCustomTreeWidget::dropEvent(QDropEvent *e)
{
    bool valid = false;
    QPoint pos = e->pos();
    QModelIndex index;
    // we test if we point inside an item
    if (pTree && e->source() == this && viewport()->rect().contains(pos))
    {
        index = indexAt(pos);
        if (!index.isValid())
        {
            return;
        }
        QTreeWidgetItem *item = itemAt(pos);
        // now we have an item, we check whether we are above, below or on it
        QRect rect = visualRect(index);
        const int margin = 2;
       
        string indices = pTree->indicesOf(dynamic_cast<QCustomTreeWidgetItem*>(pDragSource)->branch()); 
        string newIndices;
        // different positions
        if (pos.y() - rect.top() < margin)
        {
            // we are above it
            newIndices = pTree->indicesOf(dynamic_cast<QCustomTreeWidgetItem*>(item)->branch());
        }
        else if (rect.bottom() - pos.y() < margin)
        {
            // we are below it
            newIndices = pTree->indicesOfNext(dynamic_cast<QCustomTreeWidgetItem*>(item)->branch());
        }
        else if (rect.contains(pos, true))
        {
            // we are on it : new child
            stringstream buf(stringstream::in|stringstream::out);
            Branch *branch = dynamic_cast<QCustomTreeWidgetItem*>(item)->branch();
            buf << pTree->indicesOf(branch);
            // we want to add it as the last child
            buf << "_" << branch->tree().numberOfChildren();
            newIndices = buf.str();
            // we expand the parent item
            expandItem(item);
        }
        // move
        valid = pTree->move(indices, newIndices);
        // modification
        if (valid)
        {
            emit modificationDone(new TreeModification(*pTree, indices, newIndices));
        }
    }
    if (!valid)
    {
        e->setDropAction(Qt::IgnoreAction);
    }
    QTreeWidget::dropEvent(e);
    resizeColumnToContents(0);
}

void QCustomTreeWidget::on_itemSelectionChanged()
{
    bEditing = false;
}

void QCustomTreeWidget::addItem(QCustomTreeWidgetItem *item, bool edition)
{
    if (pItemDial)
    {
        if (edition)
        {
            pItemDial->exec(item->branch()->item());
        }
        else
        {
            pItemDial->exec();
        }
        if (pItemDial->result()==QDialog::Accepted)
        {
            // creation of the new item
            Item *newItem = 0;
            try
            {
                switch (pItemDial->type())
                {
                    case Item::tSound:      {
                                                SoundItem *soundItem = new SoundItem(pItemDial->text().toStdString(), pItemDial->state(), false);
                                                soundItem->setFileName(pItemDial->fileName().toStdString(), false);
                                                newItem = soundItem;
                                                break;
                                            }
                    case Item::tImage:      newItem = new ImageItem(pItemDial->text().toStdString(), pItemDial->state(), false, pItemDial->fileName().toStdString());
                                            break;
                    default:                newItem = new Item(pItemDial->text().toStdString(),pItemDial->state());
                                            break;
                }
            }
            catch (exception &e)
            {
                QMessageBox::critical(this,QApplication::translate("mainWindow","Error",0),e.what());
                if (newItem)
                {
                    delete newItem;
                }
                return;
            }
            Branch *newBranch = 0;
            if (item)
            {
                // an item is selected
                if (edition)
                {
                    Item *iItem = item->branch()->item();
                    emit modificationDone(new TreeModification(*pTree, iItem, ItemFactory::copyItem(newItem), pTree->indicesOf(item->branch())));
                    // stopping the music if necessary
                    if (pmMethod == pmMusic && iItem->type() == Item::tSound)
                    {
                        SoundItem *siItem = dynamic_cast<SoundItem*>(iItem);
                        if (newItem->type() == Item::tSound)
                        {
                            SoundItem* newSItem = dynamic_cast<SoundItem*>(newItem);
                            if (newSItem->fileName() != siItem->fileName())
                            {
                                emit fileToStop(siItem);
                            }
                            else
                            {
                                emit fileToChange(siItem, newSItem);
                            }
                        }
                        else
                        {
                            emit fileToStop(siItem);
                        }
                    }
                    // replacement
                    item->branch()->setItem(newItem);
                    item->updateDisplay(); 
                    scrollTo(item);
                }
                else
                {    
                    // new item is created
                    QCustomTreeWidgetItem *newQItem = 0;
                    switch (pItemDial->selectionResult())
                    {
                        case ItemDialog::rBrother:  {
                                                        // we want to insert it after the given item
                                                        Branch *parent = item->branch()->parent()->parent();
                                                        if (parent)
                                                        {
                                                            newBranch = parent->tree().insert(parent->tree().indexOf(item->branch())+1,newItem);
                                                            newQItem = new QCustomTreeWidgetItem(dynamic_cast<QCustomTreeWidgetItem*>(item->parent()),newBranch,item);
                                                        }
                                                        else
                                                        {
                                                            // if there is no parent, add it directly to the tree
                                                            newBranch = pTree->insert(pTree->indexOf(item->branch())+1,newItem);
                                                            newQItem = new QCustomTreeWidgetItem(this,newBranch,item);
                                                        }
                                                        break;
                                                    }
                        case ItemDialog::rChild:    {
                                                        // we want to insert it inside the given item
                                                        newBranch = item->branch()->tree().add(newItem);
                                                        newQItem = new QCustomTreeWidgetItem(item,newBranch);
                                                        expandItem(newQItem->parent());
                                                        break;
                                                    }
                    }
                    scrollTo(newQItem);
                    setCurrentItem(newQItem);
                }
            }
            else
            {
                // There is no item in the tree
                newBranch = pTree->add(newItem);
                setCurrentItem(new QCustomTreeWidgetItem(this,newBranch));
            }
            // creating the modification
            if (!edition)
            {
                emit modificationDone(new TreeModification(*pTree, ItemFactory::copyItem(newItem), pTree->indicesOf(newBranch)));
            }
            resizeColumnToContents(0);
        }
    }
}

void QCustomTreeWidget::setPlayingMethod(QWidget *player, PlayingMethod playingMethod)
{
    pmMethod = playingMethod;
    disconnect(SIGNAL(fileToPlay(const SoundItem*)), player, SLOT(playMusic(const SoundItem*)));
    disconnect(SIGNAL(fileToPlay(const SoundItem*)), player, SLOT(playSound(const SoundItem*)));
    disconnect(SIGNAL(fileToStop(const SoundItem*)), player, SLOT(stopMusic(const SoundItem*)));
    switch (playingMethod)
    {
        case pmSound:   connect(this, SIGNAL(fileToPlay(const SoundItem*)), player, SLOT(playSound(const SoundItem*)));
                        break;
        case pmMusic:   connect(this, SIGNAL(fileToPlay(const SoundItem*)), player, SLOT(playMusic(const SoundItem*)));
                        // connection for stopping
                        connect(this, SIGNAL(fileToStop(const SoundItem*)), player, SLOT(stopMusic(const SoundItem*)));
                        // connection for changing without modifying playback
                        connect(this, SIGNAL(fileToChange(const SoundItem*, const SoundItem*)), player, SLOT(changeCurrentMusic(const SoundItem*, const SoundItem*)));
                        break;
        default:        break;                        
    }
}

QCustomTreeWidget::PlayingMethod QCustomTreeWidget::playingMethod() const
{
    return pmMethod;
}

void QCustomTreeWidget::onItemExpanded(QTreeWidgetItem *item)
{
    dynamic_cast<QCustomTreeWidgetItem*>(item)->branch()->item()->setExpanded(true);
    resizeColumnToContents(0);
}

void QCustomTreeWidget::onItemCollapsed(QTreeWidgetItem *item)
{
    dynamic_cast<QCustomTreeWidgetItem*>(item)->branch()->item()->setExpanded(false);
    resizeColumnToContents(0);
}

void QCustomTreeWidget::scrollTo(QTreeWidgetItem *item)
{
    repaint();
    scrollToItem(item);

    int column_count = columnCount();
    if (!horizontalScrollBar()->isVisible() && visualItemRect(item).y() > viewport()->height() - 2*horizontalScrollBar()->height() && columnViewportPosition(column_count-1) + columnWidth(column_count-1) > viewport()->width())
    {
        QScrollBar *bar = verticalScrollBar();
        int y = bar->value() + horizontalScrollBar()->height();
        if (y > bar->maximum())
        {
            bar->setMaximum(y);
        }
        bar->setValue(y);
    }
}

void QCustomTreeWidget::updateModification(TreeModification *modification, bool undo)
{
    string indices;
    if (undo)
    {
        switch (modification->action())
        {
            case Modification::aAddition:
                {
                    indices = modification->deletedIndices();
                    // stopping music if necessary
                    Item *item = modification->undoneItem();
                    if (pmMethod == pmMusic && item->type() == Item::tSound)
                    {
                        emit fileToStop(dynamic_cast<SoundItem*>(item));
                    }
                    modification->freeUndoneItem();
                    break;
                }
            case Modification::aEdition:    if (modification->editionType() == TreeModification::etFull)
                                            {
                                                // stopping music if necessary
                                                Item *item = modification->undoneItem();
                                                if (pmMethod == pmMusic && item->type() == Item::tSound)
                                                {
                                                    SoundItem *sItem = dynamic_cast<SoundItem*>(item);
                                                    Item *oldItem = modification->currentItem();
                                                    if (oldItem->type() == Item::tSound)
                                                    {
                                                        SoundItem *oldSItem = dynamic_cast<SoundItem*>(oldItem);
                                                        if (oldSItem->fileName() != sItem->fileName())
                                                        {
                                                            emit fileToStop(sItem);
                                                        }
                                                        else
                                                        {
                                                            emit fileToChange(sItem, oldSItem);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        emit fileToStop(sItem);
                                                    }
                                                }
                                                modification->freeUndoneItem();
                                            }
            default:    indices = modification->indices();
        }
    }
    else
    {
        switch (modification->action())
        {
            case Modification::aDeletion:   indices = modification->deletedIndices();
                                            if (pmMethod == pmMusic)
                                            {
                                                stopMusic(modification->branch());
                                            }
                                            break;
            case Modification::aMovement:   indices = modification->modifiedNewIndices();
                                            break;
            case Modification::aEdition:    if (modification->editionType() == TreeModification::etFull)
                                            {
                                                // stopping music if necessary
                                                Item *item = modification->undoneItem();
                                                if (pmMethod == pmMusic && item->type() == Item::tSound)
                                                {
                                                    SoundItem *sItem = dynamic_cast<SoundItem*>(item);
                                                    Item *newItem = modification->currentItem();
                                                    if (newItem->type() == Item::tSound)
                                                    {
                                                        SoundItem *newSItem = dynamic_cast<SoundItem*>(newItem);
                                                        if (newSItem->fileName() != sItem->fileName())
                                                        {
                                                            emit fileToStop(sItem);
                                                        }
                                                        else
                                                        {
                                                            emit fileToChange(sItem, newSItem);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        emit fileToStop(sItem);
                                                    }
                                                }
                                                modification->freeUndoneItem();
                                            }
            default:    indices = modification->indices();
        }
    }
    updateDisplay(indices);
    setFocus(Qt::OtherFocusReason);
} 

void QCustomTreeWidget::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        retranslate();
    }
}

void QCustomTreeWidget::retranslate()
{
    actionNone->setText(QApplication::translate("customTree","&None",0));
    actionNone->setStatusTip(QApplication::translate("customTree","Untag the item",0));
    actionNone->setShortcut(QString("Ctrl+F5"));
    actionProgress->setText(QApplication::translate("customTree","In &progress",0));
    actionProgress->setStatusTip(QApplication::translate("customTree","Tag the item as being in progress",0));
    actionProgress->setShortcut(QString("Ctrl+F6"));
    actionFailure->setText(QApplication::translate("customTree","&Failed",0));
    actionFailure->setStatusTip(QApplication::translate("customTree","Tag the item as failed",0));
    actionFailure->setShortcut(QString("Ctrl+F7"));
    actionSuccess->setText(QApplication::translate("customTree","&Succeeded",0));
    actionSuccess->setStatusTip(QApplication::translate("customTree","Tag the item as succeeded",0));
    actionSuccess->setShortcut(QString("Ctrl+F8"));
    actionAdd->setText(QApplication::translate("customTree","&Add",0));
    actionAdd->setStatusTip(QApplication::translate("customTree","Add a new item",0));
    actionAdd->setShortcut(QString("Ins"));
    actionDelete->setText(QApplication::translate("customTree","&Remove",0));
    actionDelete->setStatusTip(QApplication::translate("customTree","Remove the item",0));
    actionDelete->setShortcut(QString("Del"));
    actionEdit->setText(QApplication::translate("customTree","&Edit",0));
    actionEdit->setStatusTip(QApplication::translate("customTree","Edit the item",0));
    actionEdit->setShortcut(QString("Ctrl+F2"));
    actionLaunch->setShortcut(QString("Space"));
    actionExport->setText(QApplication::translate("customTree", "E&xport", 0));
    actionExport->setStatusTip(QApplication::translate("customTree", "Export the file associated to the item", 0));
    actionExport->setShortcut(QString("Ctrl+Space"));

    // also retranslate the items
    for (QTreeWidgetItemIterator it(this); *it; it++)
    {
        dynamic_cast<QCustomTreeWidgetItem*>(*it)->updateDisplay();
    }
}

void QCustomTreeWidget::setItemDialogWindow(ItemDialog *window)
{
    pItemDial = window;
}

void QCustomTreeWidget::changeState(QCustomTreeWidgetItem *item, Item::State state)
{
    if (item)
    {
        item->setIcon(1, icon(state));
        if (pTree)
        {
            Item *treeItem = item->branch()->item();
            emit modificationDone(new TreeModification(*pTree, treeItem->state(), state, pTree->indicesOf(item->branch())));
            treeItem->setState(state);
        }
    }
}

void QCustomTreeWidget::exportFile(Item *item)
{
    FileItem *fileItem = dynamic_cast<FileItem*>(item);
    if (fileItem)
    {
        QString fileName = fileItem->fileName().c_str();
        QString newFileName = QFileDialog::getSaveFileName(this, QApplication::translate("customTree", "Select where to export the file", 0), "", "*." + QFileInfo(fileName).suffix(),0,QFileDialog::DontUseNativeDialog);
        if (!newFileName.isEmpty())
        {
            if (!QFile::copy(fileName, newFileName))
            {
                QMessageBox::critical(0, QApplication::translate("customTree", "Error", 0), QApplication::translate("customTree", "Unable to export the file", 0));
            }
        }
    }
}

void QCustomTreeWidget::stopMusic(Branch *branch)
{
    if (branch)
    {
        Item *item = branch->item();
        Tree &children = branch->tree();
        if (item->type() == Item::tSound)
        {
            emit fileToStop(dynamic_cast<SoundItem*>(item));
        }
        for (Tree::iterator it = children.begin(); it != children.end(); it++)
        {
            Item *childItem = it.branch()->item();
            if (childItem->type() == Item::tSound)
            {
                emit fileToStop(dynamic_cast<SoundItem*>(childItem));
            }
        }
    }
}
