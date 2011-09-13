#include "QCustomTreeWidgetItem.h"

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidget *parent, Branch *branch, QCustomTreeWidgetItem *previous): QTreeWidgetItem(parent,previous), pBranch(branch)
{
}

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidgetItem *parent, Branch *branch, QCustomTreeWidgetItem *previous): QTreeWidgetItem(parent,previous), pBranch(branch)
{
}

Branch* QCustomTreeWidgetItem::branch()
{
    return pBranch;
}
