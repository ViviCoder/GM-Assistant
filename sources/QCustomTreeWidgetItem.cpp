#include "QCustomTreeWidgetItem.h"

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidget *parent, Item *item): QTreeWidgetItem(parent), pItem(item)
{
}

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidgetItem *parent, Item *item): QTreeWidgetItem(parent), pItem(item)
{
}

Item* QCustomTreeWidgetItem::item()
{
    return pItem;
}
