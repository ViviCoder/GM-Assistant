#include "QCustomTreeWidgetItem.h"

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidget *parent, Branch *branch): QTreeWidgetItem(parent), pBranch(branch)
{
}

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidgetItem *parent, Branch *branch): QTreeWidgetItem(parent), pBranch(branch)
{
}

Branch* QCustomTreeWidgetItem::branch()
{
    return pBranch;
}
