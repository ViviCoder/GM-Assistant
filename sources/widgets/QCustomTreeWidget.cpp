#include "QCustomTreeWidget.h"
#include "QCustomTreeWidgetItem.h"
#include "ItemFactory.h"
#include <QApplication>

QCustomTreeWidget::QCustomTreeWidget(QWidget *parent): QTreeWidget(parent), menuIcons(new QMenu(this)), iFailure(":/data/images/failure.png"),iSuccess(":/data/images/check.png"),iProgress(":/data/images/uncheck.png"), pTree(NULL), pItemDial(new ItemDialog(this)) 
{
    // popup menu
    actionNone = menuIcons->addAction(QApplication::translate("custom","&None",0));
    actionProgress = menuIcons->addAction(iProgress,QApplication::translate("custom","In &progress",0));
    actionProgress->setIconVisibleInMenu(true);
    actionFailure = menuIcons->addAction(iFailure,QApplication::translate("custom","&Failed",0));
    actionFailure->setIconVisibleInMenu(true);
    actionSuccess = menuIcons->addAction(iSuccess,QApplication::translate("custom","&Succeeded",0));
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

void QCustomTreeWidget::mousePressEvent(QMouseEvent *e)
{
    QTreeWidgetItem *item = itemAt(e->pos());
    switch (e->button())
    {
        case Qt::LeftButton:    if (item == NULL || item->isSelected())
                                {
                                    setCurrentItem(NULL);
                                }
                                else
                                {
                                    QTreeWidget::mousePressEvent(e);
                                }
                                break;
        case Qt::RightButton:   if (item != NULL)
                                {
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
                                        item->setIcon(1,iProgress);
                                        treeItem->setState(Item::sProgress);
                                    }
                                    else if (action == actionFailure)
                                    {
                                        item->setIcon(1,iFailure);
                                        treeItem->setState(Item::sFailure);
                                    }
                                    else if (action == actionSuccess)
                                    {
                                        item->setIcon(1,iSuccess);
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
                                            Item *newItem = ItemFactory::createItem(Item::tBasic,pItemDial->text().toStdString(),pItemDial->state());
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
                                            newQItem->setText(0,newItem->content().c_str());;
                                            newQItem->setIcon(1,icon(newItem->state()));
                                            resizeColumnToContents(0);
                                        }
                                    }
                                }
                                else if (topLevelItemCount()==0)
                                {
                                    pItemDial->exec();
                                    if (pItemDial->result()==QDialog::Accepted)
                                    {
                                        Item *newItem = ItemFactory::createItem(Item::tBasic,pItemDial->text().toStdString(),pItemDial->state());
                                        QCustomTreeWidgetItem *newQItem = NULL;
                                        Branch *newBranch = pTree->add(newItem);
                                        newQItem = new QCustomTreeWidgetItem(this,newBranch);
                                        newQItem->setText(0,newItem->content().c_str());;
                                        newQItem->setIcon(1,icon(newItem->state()));
                                        resizeColumnToContents(0);
                                    }
                                    
                                }
                                break;
        default:    break;
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
            item->setText(0,(*it)->content().c_str());
            item->setIcon(1,icon((*it)->state()));
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

QIcon QCustomTreeWidget::icon(Item::State state) const
{
    switch (state)
    {
        case Item::sProgress: return iProgress;   break;
        case Item::sFailure:  return iFailure;    break;
        case Item::sSuccess:  return iSuccess;    break;
        default:    return QIcon(); break;
    }
}
