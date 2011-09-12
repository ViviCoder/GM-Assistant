#ifndef HEADER_QCUSTOMTREEWIDGETITEM
#define HEADER_QCUSTOMTREEWIDGETITEM

#include <QTreeWidgetItem>
#include "QCustomTreeWidget.h"
#include "Branch.h"

class QCustomTreeWidgetItem: public QTreeWidgetItem
{
    private:
        Branch* pBranch;
    public:
        QCustomTreeWidgetItem(QTreeWidget *parent, Branch *branch);
        QCustomTreeWidgetItem(QTreeWidgetItem *parent, Branch *branch);
        Branch* branch();
};

#endif
