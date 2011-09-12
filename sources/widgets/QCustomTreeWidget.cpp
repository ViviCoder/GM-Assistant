#include "QCustomTreeWidget.h"
#include "QCustomTreeWidgetItem.h"
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
                                        treeItem->setState(sNone);
                                    }
                                    else if (action == actionProgress)
                                    {
                                        item->setIcon(1,iProgress);
                                        treeItem->setState(sProgress);
                                    }
                                    else if (action == actionFailure)
                                    {
                                        item->setIcon(1,iFailure);
                                        treeItem->setState(sFailure);
                                    }
                                    else if (action == actionSuccess)
                                    {
                                        item->setIcon(1,iSuccess);
                                        treeItem->setState(sSuccess);
                                    }
                                    else if (action == actionDelete)
                                    {
                                        deleteItem(item);
                                    }
                                    else if (action == actionAdd)
                                    {
                                        pItemDial->show();
                                        if (pItemDial->result()!=ItemDialog::rCancel)
                                        {
                                            switch (pItemDial->result())
                                            {
                                                case ItemDialog::rBrother:  {
                                                                                break;
                                                                            }
                                                case ItemDialog::rChild:    {
                                                                                break;
                                                                            }
                                                default: break;
                                            }
                                        }
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
            switch ((*it)->state())
            {
                case    sProgress:  item->setIcon(1,iProgress); break;
                case    sFailure:   item->setIcon(1,iFailure); break;
                case    sSuccess:   item->setIcon(1,iSuccess); break;
                default:    break;
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
