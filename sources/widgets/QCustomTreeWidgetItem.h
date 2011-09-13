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
        QCustomTreeWidgetItem(QCustomTreeWidget *parent, Branch *branch, QCustomTreeWidgetItem *previous=NULL);
        QCustomTreeWidgetItem(QCustomTreeWidgetItem *parent, Branch *branch, QCustomTreeWidgetItem *previous=NULL);
        Branch* branch();
        // update the displat (text and icons)
        void updateDisplay();
};

#endif
