#include "QCustomTreeWidgetItem.h"
#include "ItemFactory.h"

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidget *parent, Branch *branch): QTreeWidgetItem(parent), pBranch(branch)
{
    updateDisplay();
}

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidgetItem *parent, Branch *branch): QTreeWidgetItem(parent), pBranch(branch)
{
    updateDisplay();
}

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidget *parent, Branch *branch, QCustomTreeWidgetItem *previous): QTreeWidgetItem(parent,previous), pBranch(branch)
{
    updateDisplay();
}

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidgetItem *parent, Branch *branch, QCustomTreeWidgetItem *previous): QTreeWidgetItem(parent,previous), pBranch(branch)
{
    updateDisplay();
}

Branch* QCustomTreeWidgetItem::branch()
{
    return pBranch;
}

void QCustomTreeWidgetItem::updateDisplay()
{
    Item *item = pBranch->item();
    setText(0,item->content().c_str());
    setIcon(1,QCustomTreeWidget::icon(item->state()));
    if (item->type()==Item::tSound)
    {
        setIcon(0,QIcon(":/data/images/speaker.png"));
        setToolTip(0,dynamic_cast<SoundItem*>(item)->fileName().c_str()); 
        setStatusTip(0,QApplication::translate("custom","Double click to play the file",0));
    }
}
