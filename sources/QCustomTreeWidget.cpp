#include "QCustomTreeWidget.h"

QCustomTreeWidget::QCustomTreeWidget(QWidget *parent): QTreeWidget(parent)
{
}

void QCustomTreeWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button()==Qt::LeftButton)
    {
        QTreeWidgetItem *item = itemAt(e->pos());
        if (item != NULL && item->isSelected())
        {
            setCurrentItem(NULL);
        }
        else
        {
            QTreeWidget::mousePressEvent(e);
        }
    }
}

void QCustomTreeWidget::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key()==Qt::Key_F2)
    {
        QTreeWidgetItem *item = currentItem();
        if (item != NULL && item->isSelected())
        {
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            QTreeWidget::editItem(item);
        }
    }
}
