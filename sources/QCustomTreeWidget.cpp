#include "QCustomTreeWidget.h"
#include "QCustomTreeWidgetItem.h"
#include <QApplication>

QCustomTreeWidget::QCustomTreeWidget(QWidget *parent): QTreeWidget(parent), menuIcons(new QMenu(this)), iFailure("data/images/failure.png"),iSuccess("data/images/check.png"),iProgress("data/images/uncheck.png") 
{
    // popup menu
    actionNone = menuIcons->addAction(QApplication::translate("custom","&None",0));
    actionProgress = menuIcons->addAction(iProgress,QApplication::translate("custom","In &progress",0));
    actionProgress->setIconVisibleInMenu(true);
    actionFailure = menuIcons->addAction(iFailure,QApplication::translate("custom","&Failed",0));
    actionFailure->setIconVisibleInMenu(true);
    actionSuccess = menuIcons->addAction(iSuccess,QApplication::translate("custom","&Succeeded",0));
    actionSuccess->setIconVisibleInMenu(true);
    // connecting signals
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem *,int)), SLOT(changeItem(QTreeWidgetItem*, int)));
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
                                    Item *treeItem = dynamic_cast<QCustomTreeWidgetItem*>(item)->item();
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
                                }
                                break;
        default:    break;
    }
}

void QCustomTreeWidget::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key()==Qt::Key_F2)
    {
        QTreeWidgetItem *item = currentItem();
        if (item != NULL)
        {
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            editItem(item);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        }
    }
}

void QCustomTreeWidget::paintEvent(QPaintEvent *e)
{
    for (int i=0; i<columnCount(); i++)
    {
        resizeColumnToContents(i);
    }
    QTreeWidget::paintEvent(e);
}

void QCustomTreeWidget::changeItem(QTreeWidgetItem* item, int column)
{
    if (item != NULL && column == 0)
    {
        dynamic_cast<QCustomTreeWidgetItem*>(item)->item()->setContent(item->text(0).toStdString());
    }
}

void QCustomTreeWidget::setTree(Tree *tree)
{
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
            item = new QCustomTreeWidgetItem(this, *it);
        }
        else
        {
            item = new QCustomTreeWidgetItem(items[depth-1], *it);
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
}
