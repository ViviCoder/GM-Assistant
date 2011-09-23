#include "QCustomTableWidget.h"
#include "ChangeHeaderDialog.h"
#include <QApplication>

QCustomTableWidget::QCustomTableWidget(QWidget *parent): QTableWidget(parent), menuColumn(new QMenu(this)), pChangeHeaderDial(new ChangeHeaderDialog(this))
{
    //popup menu
    //actionNone = menuColumn->addAction(QApplication::translate("custom","&None",0));
    actionAddColumn = menuColumn->addAction(QIcon(":/data/images/add.png"),QApplication::translate("custom","&Add a skill",0));
    actionAddColumn->setIconVisibleInMenu(true);
    actionRemoveColumn = menuColumn->addAction(QIcon(":/data/images/remove.png"),QApplication::translate("custom","&Remove the skill",0));
    actionRemoveColumn->setIconVisibleInMenu(true);
    actionEditColumn = menuColumn->addAction(QIcon(":/data/images/son.png"),QApplication::translate("custom","Change &skill's name",0));
    actionEditColumn->setIconVisibleInMenu(true);
    menuColumn->addSeparator();
    actionAddRow = menuColumn->addAction(QIcon(":/data/images/add.png"),QApplication::translate("custom","A&dd a character",0));
    actionAddRow->setIconVisibleInMenu(true);
    actionRemoveRow = menuColumn->addAction(QIcon(":/data/images/remove.png"),QApplication::translate("custom","R&emove the character",0));
    actionRemoveRow->setIconVisibleInMenu(true);
    actionEditRow = menuColumn->addAction(QIcon(":/data/images/son.png"),QApplication::translate("custom","Change &character's name",0));
    actionEditRow->setIconVisibleInMenu(true);
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
        case Qt::LeftButton:    {
                                    if (qtwitem == NULL || qtwitem->isSelected())
                                    {
                                        setCurrentItem(NULL);
                                    }
                                    else
                                    {
                                        QTableWidget::mousePressEvent(e);
                                    }
                                    break;
                                }
        case Qt::RightButton:   {
                                    if (qtwitem == NULL)
                                    {
                                        rowPosition = -1;
                                        columnPosition = -1;
                                    }
                                    else
                                    {
                                        rowPosition = currentRow();
                                        columnPosition = currentColumn();
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
                                            removeRow(rowPosition);
                                        }
                                    }
                                    if (action == actionEditColumn)
                                    {
                                        if (qtwitem != NULL)
                                        {
                                            if(pChangeHeaderDial->exec()==QDialog::Accepted)
                                            {
                                                QTableWidgetItem *columnHeaderItem = horizontalHeaderItem ( columnPosition );
                                                if (columnHeaderItem != NULL)
                                                {
                                                    columnHeaderItem->setText(pChangeHeaderDial->text());
                                                }
                                                else
                                                {
                                                    columnHeaderItem = new QTableWidgetItem(pChangeHeaderDial->text());
                                                    setHorizontalHeaderItem(columnPosition, columnHeaderItem);
                                                }
                                            }
                                        }

                                    }
                                    if (action == actionEditRow)
                                    {
                                        if (qtwitem != NULL)
                                        {
                                            if(pChangeHeaderDial->exec()==QDialog::Accepted)
                                            {
                                                QTableWidgetItem *rowHeaderItem = verticalHeaderItem ( rowPosition );
                                                if (rowHeaderItem != NULL)
                                                {
                                                    rowHeaderItem->setText(pChangeHeaderDial->text());
                                                }
                                                else
                                                {
                                                    rowHeaderItem = new QTableWidgetItem(pChangeHeaderDial->text());
                                                    setVerticalHeaderItem(rowPosition, rowHeaderItem);
                                                }
                                            }
                                        }

                                    }
                                    break;
                                }
        default:    break;
    }
}


void QCustomTableWidget::keyReleaseEvent(QKeyEvent *e)
{
    QTableWidgetItem *item = currentItem();
    if (item != NULL)
    {
        switch (e->key())
        {
            case Qt::Key_F2:    item->setFlags(item->flags() | Qt::ItemIsEditable);
                                editItem(item);
                                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                                break;
            default: break; 
        }
    }
}

