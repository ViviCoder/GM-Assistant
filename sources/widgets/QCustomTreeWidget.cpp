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

#include "QCustomTreeWidget.h"
#include "QCustomTreeWidgetItem.h"
#include "ItemFactory.h"
#include "SoundEngine.h"
#include "PictureWindow.h"
#include <QApplication>
#include <QMessageBox>
#include <exception>

QCustomTreeWidget::QCustomTreeWidget(QWidget *parent): QTreeWidget(parent), menuIcons(new QMenu(this)), pTree(NULL), pItemDial(new ItemDialog(this)), pDragSource(NULL), bNewlySelected(false), bEditing(false)
{
    // creating actions
    actionNone = new QAction(QIcon(":/data/images/empty.png"),QApplication::translate("customTree","&None",0),this);
    actionNone->setIconVisibleInMenu(true);
    actionNone->setStatusTip(QApplication::translate("customTree","Untag the item",0));
    actionProgress = new QAction(QIcon(":/data/images/uncheck.png"),QApplication::translate("customTree","In &progress",0),this);
    actionProgress->setIconVisibleInMenu(true);
    actionProgress->setStatusTip(QApplication::translate("customTree","Tag the item as being in progress",0));
    actionFailure = new QAction(QIcon(":/data/images/failure.png"),QApplication::translate("customTree","&Failed",0),this);
    actionFailure->setIconVisibleInMenu(true);
    actionFailure->setStatusTip(QApplication::translate("customTree","Tag the item as failed",0));
    actionSuccess = new QAction(QIcon(":/data/images/check.png"),QApplication::translate("customTree","&Succeeded",0),this);
    actionSuccess->setIconVisibleInMenu(true);
    actionSuccess->setStatusTip(QApplication::translate("customTree","Tag the item as succeeded",0));
    actionAdd = new QAction(QIcon(":/data/images/add.png"),QApplication::translate("customTree","&Add",0),this);
    actionAdd->setIconVisibleInMenu(true);
    actionAdd->setStatusTip(QApplication::translate("customTree","Add a new item",0));
    actionAdd->setShortcut(QApplication::translate("customTree","+",0));
    actionDelete = new QAction(QIcon(":/data/images/remove.png"),QApplication::translate("customTree","&Delete",0),this);
    actionDelete->setIconVisibleInMenu(true);
    actionDelete->setStatusTip(QApplication::translate("customTree","Delete the item",0));
    actionDelete->setShortcut(QApplication::translate("customTree","Del",0));
    // populating the pop-up menu
    menuIcons->addAction(actionNone);
    menuIcons->addAction(actionProgress);
    menuIcons->addAction(actionFailure);
    menuIcons->addAction(actionSuccess);
    menuIcons->addSeparator();
    menuIcons->addAction(actionAdd);
    menuIcons->addAction(actionDelete);
    // connecting signals
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem *,int)), SLOT(on_itemChanged(QTreeWidgetItem*, int)));
    connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem *)), SLOT(on_itemCollapsed()));
    connect(this, SIGNAL(itemExpanded(QTreeWidgetItem *)), SLOT(on_itemExpanded()));
    connect(this, SIGNAL(itemSelectionChanged()), SLOT(on_itemSelectionChanged()));
}

QCustomTreeWidget::~QCustomTreeWidget()
{
    delete menuIcons;
}

void QCustomTreeWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    QTreeWidgetItem *qtwItem = itemAt(e->pos());
    if (qtwItem != NULL)
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
                                // we send a signal to play the music (and do some other things)
                                emit fileToPlay(soundItem->fileName(),soundItem->duration());
                                break;
                            }
        case Item::tPicture: {
                                PictureItem *pictureItem = dynamic_cast<PictureItem*>(item);
                                new PictureWindow(pictureItem->fileName(),this);
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
        case Qt::LeftButton:    if (item !=NULL)
                                {
                                    bNewlySelected = !item->isSelected();
                                    // the item will be selected (and not be unselected by mouseReleaseEvent)
                                }
                                QTreeWidget::mousePressEvent(e);
                                break;
        case Qt::RightButton:   if (item != NULL)
                                {
                                    setCurrentItem(item);
                                    QCustomTreeWidgetItem *qItem = dynamic_cast<QCustomTreeWidgetItem*>(item);
                                    Item *treeItem = qItem->branch()->item();
                                    QAction* action = menuIcons->exec(e->globalPos());
                                    if (action == actionNone)
                                    {
                                        item->setIcon(1,QIcon());
                                        treeItem->setState(Item::sNone);
                                    }
                                    else if (action == actionProgress)
                                    {
                                        item->setIcon(1,action->icon());
                                        treeItem->setState(Item::sProgress);
                                    }
                                    else if (action == actionFailure)
                                    {
                                        item->setIcon(1,action->icon());
                                        treeItem->setState(Item::sFailure);
                                    }
                                    else if (action == actionSuccess)
                                    {
                                        item->setIcon(1,action->icon());
                                        treeItem->setState(Item::sSuccess);
                                    }
                                    else if (action == actionDelete)
                                    {
                                        deleteItem(item);
                                    }
                                    else if (action == actionAdd)
                                    {
                                        addItem(qItem);
                                    }
                                }
                                else if (topLevelItemCount()==0)
                                {
                                    pItemDial->exec();
                                    if (pItemDial->result()==QDialog::Accepted)
                                    {
                                        Item *newItem = ItemFactory::createItem(pItemDial->type(),pItemDial->text().toStdString(),pItemDial->state());
                                        if (newItem->type()==Item::tSound)
                                        {
                                            dynamic_cast<SoundItem*>(newItem)->setFileName(pItemDial->fileName().toStdString());
                                        }
                                        if (newItem->type()==Item::tPicture)
                                        {
                                            dynamic_cast<PictureItem*>(newItem)->setFileName(pItemDial->fileName().toStdString());
                                        }
                                        Branch *newBranch = pTree->add(newItem);
                                        new QCustomTreeWidgetItem(this,newBranch);
                                        resizeColumnToContents(0);
                                    }
                                    
                                }
                                break;
        default:    break;
    }
}

void QCustomTreeWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (!bNewlySelected)
    {
        setCurrentItem(NULL);
    }
}

void QCustomTreeWidget::keyReleaseEvent(QKeyEvent *e)
{
    QTreeWidgetItem *qItem = currentItem();
    if (qItem != NULL)
    {
        switch (e->key())
        {
            case Qt::Key_F2:    qItem->setFlags(qItem->flags() | Qt::ItemIsEditable);
                                editItem(qItem);
                                bEditing = true;
                                qItem->setFlags(qItem->flags() & ~Qt::ItemIsEditable);
                                break;
            case Qt::Key_Delete:    if (!bEditing)
                                    {
                                        deleteItem(qItem);
                                    }
                                    break;
            case Qt::Key_Plus:  if (!bEditing)
                                {
                                    addItem(dynamic_cast<QCustomTreeWidgetItem*>(qItem));
                                }
                                break;
            case Qt::Key_Space: if (!bEditing)
                                {
                                    launchItem(qItem);
                                }
                                break;
            case Qt::Key_Return:
            case Qt::Key_Enter:
            case Qt::Key_Escape:    bEditing = false;
                                    break;
            default:            QTreeWidget::keyReleaseEvent(e); break; 
        }
    }
}

void QCustomTreeWidget::on_itemChanged(QTreeWidgetItem* item, int column)
{
    if (item != NULL && column == 0)
    {
        dynamic_cast<QCustomTreeWidgetItem*>(item)->branch()->item()->setContent(item->text(0).toStdString());
        resizeColumnToContents(0);
    }
}

void QCustomTreeWidget::on_itemCollapsed()
{
    resizeColumnToContents(0);
}

void QCustomTreeWidget::on_itemExpanded()
{
    resizeColumnToContents(0);
}

void QCustomTreeWidget::setTree(Tree *tree)
{
    pTree = tree;
    clear();
    setColumnCount(2);
    if (tree != NULL)
    {
        std::vector<QCustomTreeWidgetItem*> items;
        QCustomTreeWidgetItem* item;
        int depth=0;        
        // iterating the tree to populate the widget
        for (Tree::iterator it=tree->begin(); it != tree->end(); it++)
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
        }
    }
    resizeColumnToContents(0);
    resizeColumnToContents(1);
}

void QCustomTreeWidget::deleteItem(QTreeWidgetItem *item)
{
    Branch *branch = dynamic_cast<QCustomTreeWidgetItem*>(item)->branch();
    // delete item
    Tree *parent = branch->parent();
    if (parent==NULL)
    {
        if (pTree != NULL)
        {
            pTree->remove(pTree->indexOf(branch));
        }
    }
    else
    {
        parent->remove(parent->indexOf(branch));
    }
    // delete widgetItem
    delete item;
    resizeColumnToContents(0);
}

QIcon QCustomTreeWidget::icon(Item::State state)
{
    switch (state)
    {
        case Item::sProgress: return QIcon(":/data/images/uncheck.png");    break;
        case Item::sFailure:  return QIcon(":/data/images/failure.png");    break;
        case Item::sSuccess:  return QIcon(":/data/images/check.png");      break;
        default:    return QIcon(); break;
    }
}

void QCustomTreeWidget::dragEnterEvent(QDragEnterEvent *e)
{
    pDragSource = itemAt(e->pos());
    if (pDragSource != NULL)
    {
        e->acceptProposedAction();
    }
}

using namespace std;

void QCustomTreeWidget::dropEvent(QDropEvent *e)
{
    QPoint pos = e->pos();
    QModelIndex index;
    // we test if we point inside an item
    if (e->source() == this && viewport()->rect().contains(pos),true)
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
        if (pos.y() - rect.top() < margin)
        {
            // we are above it
            pTree->move(pTree->indicesOf(dynamic_cast<QCustomTreeWidgetItem*>(pDragSource)->branch()),pTree->indicesOf(dynamic_cast<QCustomTreeWidgetItem*>(item)->branch()));
        }
        else if (rect.bottom() - pos.y() < margin)
        {
            // we are below it
            pTree->move(pTree->indicesOf(dynamic_cast<QCustomTreeWidgetItem*>(pDragSource)->branch()),pTree->indicesOfNext(dynamic_cast<QCustomTreeWidgetItem*>(item)->branch()));
        }
        else if (rect.contains(pos, true))
        {
            // we are on it : new child
            stringstream buf(stringstream::in|stringstream::out);
            Branch *branch = dynamic_cast<QCustomTreeWidgetItem*>(item)->branch();
            buf << pTree->indicesOf(branch);
            // we want to add it as the last child
            buf << "_" << branch->tree().numberOfChildren();
            pTree->move(pTree->indicesOf(dynamic_cast<QCustomTreeWidgetItem*>(pDragSource)->branch()), buf.str());
        }
    }
    QTreeWidget::dropEvent(e);
    resizeColumnToContents(0);
}

void QCustomTreeWidget::on_itemSelectionChanged()
{
    bEditing = false;
}

void QCustomTreeWidget::addItem(QCustomTreeWidgetItem *item)
{
    pItemDial->exec();
    if (pItemDial->result()==QDialog::Accepted)
    {
        Item *newItem = ItemFactory::createItem(pItemDial->type(),pItemDial->text().toStdString(),pItemDial->state());
        if (newItem->type()==Item::tSound)
        {
            dynamic_cast<SoundItem*>(newItem)->setFileName(pItemDial->fileName().toStdString());
        }
        if (newItem->type()==Item::tPicture)
        {
            dynamic_cast<PictureItem*>(newItem)->setFileName(pItemDial->fileName().toStdString());
        }
        switch (pItemDial->selectionResult())
        {
            case ItemDialog::rBrother:  {
                                            Branch *parent = item->branch()->parent()->parent();
                                            if (parent==NULL)
                                            {
                                                Branch *newBranch = pTree->insert(pTree->indexOf(item->branch())+1,newItem);
                                                new QCustomTreeWidgetItem(this,newBranch,item);
                                            }
                                            else
                                            {
                                                Branch *newBranch = parent->tree().insert(parent->tree().indexOf(item->branch())+1,newItem);
                                                new QCustomTreeWidgetItem(dynamic_cast<QCustomTreeWidgetItem*>(item->parent()),newBranch,item);
                                            }
                                            break;
                                        }
            case ItemDialog::rChild:    {
                                            Branch *newBranch = item->branch()->tree().add(newItem);
                                            QCustomTreeWidgetItem *newQItem = new QCustomTreeWidgetItem(item,newBranch);
                                            expandItem(newQItem->parent());
                                            break;
                                        }
        }
        resizeColumnToContents(0);
    }
}
