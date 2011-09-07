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
