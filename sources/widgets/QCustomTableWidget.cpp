/*************************************************************************
* Copyright © 2011 Vincent Prat & Simon Nicolas
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*************************************************************************/

#include "QCustomTableWidget.h"
#include "ChangeSkillDialog.h"
#include <QApplication>

QCustomTableWidget::QCustomTableWidget(QWidget *parent): QTableWidget(parent), menuColumn(new QMenu(this)), pChangeSkillDial(new ChangeSkillDialog(this)), pChangeCharacterDial(new ChangeCharacterDialog(this)), pSkills(NULL), pCharacters(NULL)
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
                                        setCurrentItem(NULL);

                                        int row_nb;
                                        row_nb = rowCount();
                                        rowPosition = row_nb -1;

                                        int column_nb;
                                        column_nb = columnCount();
                                        columnPosition = column_nb -1;
                                    }
                                    else
                                    {
                                        QTableWidget::mousePressEvent(e);
                                        rowPosition = currentRow();
                                        columnPosition = currentColumn();
                                    }
                                        
                                    QAction* action = menuColumn->exec(e->globalPos());
                                    if (action == actionAddColumn)
                                    {
                                        insertColumn(columnPosition+1);
                                        int row_nb;
                                        row_nb = rowCount();
                                        for (int i = 0; i <= row_nb; i++)
                                        {
                                            QTableWidgetItem *col1 = new QTableWidgetItem( "0" );
                                            setItem(i,columnPosition+1,col1);
                                        }
                                        if(pChangeSkillDial->exec()==QDialog::Accepted)
                                        {
                                            QTableWidgetItem *columnHeaderItem = horizontalHeaderItem ( columnPosition+1 );
                                            if (columnHeaderItem != NULL)
                                            {
                                                columnHeaderItem->setText(pChangeSkillDial->text());
                                            }
                                            else
                                            {
                                                columnHeaderItem = new QTableWidgetItem(pChangeSkillDial->text());
                                                setHorizontalHeaderItem(columnPosition+1, columnHeaderItem);
                                            }
                                        }
                                        resizeColumnToContents(columnPosition+1);
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
                                        int column_nb;
                                        column_nb = columnCount();
                                        for (int i = 0; i <= column_nb; i++)
                                        {
                                            QTableWidgetItem *row1 = new QTableWidgetItem( "0" );
                                            setItem(rowPosition+1,i,row1);
                                        }
                                        if(pChangeCharacterDial->exec()==QDialog::Accepted)
                                        {
                                            QTableWidgetItem *rowHeaderItem = verticalHeaderItem ( rowPosition+1 );
                                            if (rowHeaderItem != NULL)
                                            {
                                                rowHeaderItem->setText(pChangeCharacterDial->text());
                                            }
                                            else
                                            {
                                                rowHeaderItem = new QTableWidgetItem(pChangeCharacterDial->text());
                                                setVerticalHeaderItem(rowPosition+1, rowHeaderItem);
                                            }
                                        }
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
                                            if(pChangeSkillDial->exec()==QDialog::Accepted)
                                            {
                                                QTableWidgetItem *columnHeaderItem = horizontalHeaderItem ( columnPosition );
                                                if (columnHeaderItem != NULL)
                                                {
                                                    columnHeaderItem->setText(pChangeSkillDial->text());
                                                }
                                                else
                                                {
                                                    columnHeaderItem = new QTableWidgetItem(pChangeSkillDial->text());
                                                    setHorizontalHeaderItem(columnPosition, columnHeaderItem);
                                                }
                                            }
                                        }
                                    }
                                    if (action == actionEditRow)
                                    {
                                        if (qtwitem != NULL)
                                        {
                                            if(pChangeCharacterDial->exec()==QDialog::Accepted)
                                            {
                                                QTableWidgetItem *rowHeaderItem = verticalHeaderItem ( rowPosition );
                                                if (rowHeaderItem != NULL)
                                                {
                                                    rowHeaderItem->setText(pChangeCharacterDial->text());
                                                }
                                                else
                                                {
                                                    rowHeaderItem = new QTableWidgetItem(pChangeCharacterDial->text());
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
            case Qt::Key_Delete: item->setText("0"); break;
            default: break; 
        }
    }
}

void QCustomTableWidget::setLists(SkillList *skills, CharacterList *chars)
{
    pSkills = skills;
    pCharacters = chars;
    clear();
    setColumnCount(0);
    setRowCount(0);
    int i=0;
    for (SkillList::iterator it = skills->begin(); it != skills->end(); it++)
    {
        insertColumn(i);
        setHorizontalHeaderItem(i,new QTableWidgetItem((*it).c_str()));
        i++;
    }
    int j=0,k;
    for (CharacterList::iterator it = chars->begin(); it != chars->end(); it++)
    {
        insertRow(j);
        setVerticalHeaderItem(j,new QTableWidgetItem(((*it).name()+"\n"+(*it).playerName()).c_str()));
        // creating items
        for (k=0;k<i;k++)
        {
            setItem(j,k,new QTableWidgetItem("0"));
        }
        // setting values
        k = 0;
        for (Character::SkillIterator itSkill = (*it).begin(); itSkill != (*it).end() && k<i; itSkill++)
        {
            item(j,k)->setText((*itSkill).c_str());
            k++;
        } 
        j++;
    }
    resizeRowsToContents();
    resizeColumnsToContents();
}
