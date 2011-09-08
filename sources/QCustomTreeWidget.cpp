#include "QCustomTreeWidget.h"
#include <QApplication>

QCustomTreeWidget::QCustomTreeWidget(QWidget *parent): QTreeWidget(parent), menuIcons(new QMenu(this)), iFailure("data/images/failure.png"),iSuccess("data/images/check.png"),iProgress("data/images/uncheck.png") 
{
    actionNone = menuIcons->addAction(QApplication::translate("custom","&None",0));
    actionProgress = menuIcons->addAction(iProgress,QApplication::translate("custom","In &progress",0));
    actionProgress->setIconVisibleInMenu(true);
    actionFailure = menuIcons->addAction(iFailure,QApplication::translate("custom","&Failed",0));
    actionFailure->setIconVisibleInMenu(true);
    actionSuccess = menuIcons->addAction(iSuccess,QApplication::translate("custom","&Succeeded",0));
    actionSuccess->setIconVisibleInMenu(true);
}

QCustomTreeWidget::~QCustomTreeWidget()
{
    delete menuIcons;
}

void QCustomTreeWidget::mousePressEvent(QMouseEvent *e)
{
    QTreeWidgetItem *item = itemAt(e->pos());
    switch (e->button())
    {
        case Qt::LeftButton:    if (item == NULL || item->isSelected())
                                {
                                    setCurrentItem(NULL);
                                }
                                else
                                {
                                    QTreeWidget::mousePressEvent(e);
                                }
                                break;
        case Qt::RightButton:   if (item != NULL)
                                {
                                    QAction* action = menuIcons->exec(e->globalPos());
                                    if (action == actionNone)
                                    {
                                        item->setIcon(1,QIcon());
                                    }
                                    else if (action == actionProgress)
                                    {
                                        item->setIcon(1,iProgress);
                                    }
                                    else if (action == actionFailure)
                                    {
                                        item->setIcon(1,iFailure);
                                    }
                                    else if (action == actionSuccess)
                                    {
                                        item->setIcon(1,iSuccess); break;
                                    }
                                }
        default:    break;
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
