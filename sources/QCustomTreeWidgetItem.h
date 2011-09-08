#ifndef HEADER_QCUSTOMTREEWIDGETITEM
#define HEADER_QCUSTOMTREEWIDGETITEM

#include <QTreeWidgetItem>
#include "QCustomTreeWidget.h"
#include "Item.h"

class QCustomTreeWidgetItem: public QTreeWidgetItem
{
    private:
        Item* pItem;
    public:
        QCustomTreeWidgetItem(QCustomTreeWidget *parent, Item *item);
        QCustomTreeWidgetItem(QCustomTreeWidgetItem *parent, Item *item);
        Item* item();
};

#endif
