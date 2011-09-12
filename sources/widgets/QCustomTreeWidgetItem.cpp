#include "QCustomTreeWidgetItem.h"

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QTreeWidget *parent, Branch *branch): QTreeWidgetItem(parent), pBranch(branch)
{
}

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QTreeWidgetItem *parent, Branch *branch): QTreeWidgetItem(parent), pBranch(branch)
{
}

Branch* QCustomTreeWidgetItem::branch()
{
    return pBranch;
}
