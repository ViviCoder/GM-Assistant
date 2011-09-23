#include "QCustomTableWidget.h"
#include <QApplication>

QCustomTableWidget::QCustomTableWidget(QWidget *parent): QTableWidget(parent), menuColumn(new QMenu(this))
{
    //popup menu
    actionNone = menuColumn->addAction(QApplication::translate("custom","&None",0));
    actionAddColumn = menuColumn->addAction(QIcon(":/data/images/add.png"),QApplication::translate("custom","&Add column",0));
    actionAddColumn->setIconVisibleInMenu(true);
    actionRemoveColumn = menuColumn->addAction(QIcon(":/data/images/remove.png"),QApplication::translate("custom","&Remove column",0));
    actionRemoveColumn->setIconVisibleInMenu(true);
    actionAddRow = menuColumn->addAction(QIcon(":/data/images/add.png"),QApplication::translate("custom","&Add row",0));
    actionAddRow->setIconVisibleInMenu(true);
    actionRemoveRow = menuColumn->addAction(QIcon(":/data/images/remove.png"),QApplication::translate("custom","&Remove row",0));
    actionRemoveRow->setIconVisibleInMenu(true);
}

QCustomTableWidget::~QCustomTableWidget()
{
    delete menuColumn;
}


void QCustomTableWidget::mousePressEvent(QMouseEvent *e)
{
    int rowPosition, columnPosition;
    QTableWidgetItem *qtwitem = itemAt(e->pos());
    switch (e->button())
    {
        case Qt::LeftButton:    if (qtwitem == NULL || qtwitem->isSelected())
                                {
                                    setCurrentItem(NULL);
                                }
                                else
                                {
                                    QTableWidget::mousePressEvent(e);
                                }
                                break;
        case Qt::RightButton:   if (qtwitem != NULL)
                                {
                                    rowPosition = 0;
                                    columnPosition = 0;
                                }
                                else
                                {
                                    rowPosition = QTableWidget::currentRow();
                                    columnPosition = QTableWidget::currentColumn();
                                }
                                    
                                QAction* action = menuColumn->exec(e->globalPos());
                                if (action == actionAddColumn)
                                {
                                    insertColumn(columnPosition+1);
                                }
                                if (action == actionRemoveColumn)
                                {
                                    if (qtwitem != NULL)
                                    {
                                        removeColumn(columnPosition);
                                    }
                                }
                                if (action == actionAddRow)
                                {
                                    insertRow(rowPosition+1);
                                }
                                if (action == actionRemoveRow)
                                {
                                    if (qtwitem != NULL)
                                    {
                                        insertRow(rowPosition);
                                    }
                                }
    }
}
