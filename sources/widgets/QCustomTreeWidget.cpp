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
#include <QApplication>
#include <QMessageBox>
#include <exception>

QCustomTreeWidget::QCustomTreeWidget(QWidget *parent): QTreeWidget(parent), menuIcons(new QMenu(this)), pTree(NULL), pItemDial(new ItemDialog(this)), pDragSource(NULL), bNewlySelected(false)
{
    // popup menu
    actionNone = menuIcons->addAction(QApplication::translate("custom","&None",0));
    actionProgress = menuIcons->addAction(QIcon(":/data/images/uncheck.png"),QApplication::translate("custom","In &progress",0));
    actionProgress->setIconVisibleInMenu(true);
    actionFailure = menuIcons->addAction(QIcon(":/data/images/failure.png"),QApplication::translate("custom","&Failed",0));
    actionFailure->setIconVisibleInMenu(true);
    actionSuccess = menuIcons->addAction(QIcon(":/data/images/check.png"),QApplication::translate("custom","&Succeeded",0));
    actionSuccess->setIconVisibleInMenu(true);
    menuIcons->addSeparator();
    actionAdd = menuIcons->addAction(QIcon(":/data/images/add.png"),QApplication::translate("custom","&Add",0));
    actionAdd->setIconVisibleInMenu(true);
    actionDelete = menuIcons->addAction(QIcon(":/data/images/remove.png"),QApplication::translate("custom","&Delete",0));
    actionDelete->setIconVisibleInMenu(true);
    // connecting signals
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem *,int)), SLOT(on_itemChanged(QTreeWidgetItem*, int)));
    connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem *)), SLOT(on_itemCollapsed()));
    connect(this, SIGNAL(itemExpanded(QTreeWidgetItem *)), SLOT(on_itemExpanded()));
}

QCustomTreeWidget::~QCustomTreeWidget()
{
    delete menuIcons;
}

void QCustomTreeWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    QTreeWidgetItem *qtwitem = itemAt(e->pos());
    if (qtwitem != NULL)
    {
        QCustomTreeWidgetItem *qctwitem = dynamic_cast<QCustomTreeWidgetItem*>(qtwitem);
        Item *item = qctwitem->branch()->item();
        switch (item->type())
        {
            case Item::tSound:  {
                                    SoundItem *soundItem = dynamic_cast<SoundItem*>(item);
                                    // we send a signal to play the music (and do some other things)
                                    emit fileToPlay(soundItem->fileName());
                                }
            case Item::tBasic: break;
        }
    }
}

void QCustomTreeWidget::mousePressEvent(QMouseEvent *e)
{
    QTreeWidgetItem *item = itemAt(e->pos());
    switch (e->button())
    {
        case Qt::LeftButton:    bNewlySelected = !item->isSelected();
                                // the item will be selected (and not be unselected by mouseReleaseEvent)
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
                                        pItemDial->exec();
                                        if (pItemDial->result()==QDialog::Accepted)
                                        {
                                            Item *newItem = ItemFactory::createItem(pItemDial->type(),pItemDial->text().toStdString(),pItemDial->state());
                                            if (newItem->type()==Item::tSound)
                                            {
                                                dynamic_cast<SoundItem*>(newItem)->setFileName(pItemDial->fileName().toStdString());
                                            }
                                            QCustomTreeWidgetItem *newQItem = NULL;
                                            switch (pItemDial->selectionResult())
                                            {
                                                case ItemDialog::rBrother:  {
                                                                                Branch *branch = qItem->branch()->parent();
                                                                                if (branch==NULL)
                                                                                {
                                                                                    Branch *newBranch = pTree->insert(pTree->indexOf(qItem->branch())+1,newItem);
                                                                                    newQItem = new QCustomTreeWidgetItem(this,newBranch,qItem);
                                                                                }
                                                                                else
                                                                                {
                                                                                    Branch *newBranch = branch->tree().insert(branch->tree().indexOf(qItem->branch())+1,newItem);
                                                                                    newQItem = new QCustomTreeWidgetItem(dynamic_cast<QCustomTreeWidgetItem*>(qItem->parent()),newBranch,qItem);
                                                                                }
                                                                                break;
                                                                            }
                                                case ItemDialog::rChild:    {
                                                                                Branch *newBranch = qItem->branch()->tree().add(newItem);
                                                                                newQItem = new QCustomTreeWidgetItem(qItem,newBranch);
                                                                                expandItem(newQItem->parent());
                                                                                break;
                                                                            }
                                            }
                                            resizeColumnToContents(0);
                                        }
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
                                        QCustomTreeWidgetItem *newQItem = NULL;
                                        Branch *newBranch = pTree->add(newItem);
                                        newQItem = new QCustomTreeWidgetItem(this,newBranch);
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
    QTreeWidgetItem *item = currentItem();
    if (item != NULL)
    {
        switch (e->key())
        {
            case Qt::Key_F2:    item->setFlags(item->flags() | Qt::ItemIsEditable);
                                editItem(item);
                                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                                break;
            case Qt::Key_Delete:    deleteItem(item);
                                    break;
            default: break; 
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
    Branch *parent = branch->parent();
    if (parent==NULL)
    {
        if (pTree != NULL)
        {
            pTree->remove(pTree->indexOf(branch));
        }
    }
    else
    {
        parent->tree().remove(parent->tree().indexOf(branch));
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
}
