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
#include "QCustomHeaderView.h"
#include "ChangeSkillDialog.h"
#include <QApplication>

QCustomTableWidget::QCustomTableWidget(QWidget *parent): QTableWidget(parent), menu(new QMenu(this)), hMenu(new QMenu(this)), vMenu(new QMenu(this)), pChangeSkillDial(new ChangeSkillDialog(this)), pChangeCharacterDial(new ChangeCharacterDialog(this)), pSkills(NULL), pCharacters(NULL)
{
    // popup menu
    // skills
    QMenu *menuColumn = menu->addMenu(QApplication::translate("customTable","&Skill",0));
    actionAddColumn = menuColumn->addAction(QIcon(":/data/images/add.png"),QApplication::translate("customTable","&Add",0));
    actionAddColumn->setIconVisibleInMenu(true);
    actionRemoveColumn = menuColumn->addAction(QIcon(":/data/images/remove.png"),QApplication::translate("customTable","&Remove",0));
    actionRemoveColumn->setIconVisibleInMenu(true);
    actionEditColumn = menuColumn->addAction(QIcon(":/data/images/son.png"),QApplication::translate("customTable","&Edit",0));
    actionEditColumn->setIconVisibleInMenu(true);
    // characters
    QMenu *menuRow = menu->addMenu(QApplication::translate("customTable","&Character",0));
    actionAddRow = menuRow->addAction(QIcon(":/data/images/add.png"),QApplication::translate("customTable","&Add",0));
    actionAddRow->setIconVisibleInMenu(true);
    actionRemoveRow = menuRow->addAction(QIcon(":/data/images/remove.png"),QApplication::translate("customTable","&Remove",0));
    actionRemoveRow->setIconVisibleInMenu(true);
    actionEditRow = menuRow->addAction(QIcon(":/data/images/son.png"),QApplication::translate("customTable","&Edit",0));
    actionEditRow->setIconVisibleInMenu(true);

    // popup menu for vertical header
    actionVAdd = vMenu->addAction(QIcon(":/data/images/add.png"),QApplication::translate("customTable","&Add a character",0));
    actionVAdd->setIconVisibleInMenu(true);
    actionVRemove = vMenu->addAction(QIcon(":/data/images/remove.png"),QApplication::translate("customTable","&Remove the character",0));
    actionVRemove->setIconVisibleInMenu(true);
    actionVEdit = vMenu->addAction(QIcon(":/data/images/son.png"),QApplication::translate("customTable","&Edit the character",0));
    actionVEdit->setIconVisibleInMenu(true);

    // popup menu for horizontal header
    actionHAdd = hMenu->addAction(QIcon(":/data/images/add.png"),QApplication::translate("customTable","&Add a skill",0));
    actionHAdd->setIconVisibleInMenu(true);
    actionHRemove = hMenu->addAction(QIcon(":/data/images/remove.png"),QApplication::translate("customTable","&Remove the skill",0));
    actionHRemove->setIconVisibleInMenu(true);
    actionHEdit = hMenu->addAction(QIcon(":/data/images/son.png"),QApplication::translate("customTable","&Edit the skill",0));
    actionHEdit->setIconVisibleInMenu(true);

    // headers
    setHorizontalHeader(new QCustomHeaderView(Qt::Horizontal,this));
    setVerticalHeader(new QCustomHeaderView(Qt::Vertical,this));

    // connection of signals
    connect(this,SIGNAL(cellChanged(int,int)),this,SLOT(onCellChanged(int,int)));
    connect(dynamic_cast<QCustomHeaderView*>(horizontalHeader()),SIGNAL(rightClicked(int, const QPoint&)),this,SLOT(onHHeaderClicked(int, const QPoint&)));
    connect(dynamic_cast<QCustomHeaderView*>(verticalHeader()),SIGNAL(rightClicked(int, const QPoint&)),this,SLOT(onVHeaderClicked(int, const QPoint&)));
}

QCustomTableWidget::~QCustomTableWidget()
{
    delete menu;
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
                                        if (rowCount()!=0 && columnCount()!=0)
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            rowPosition = rowCount()-1;
                                            columnPosition = columnCount()-1;
                                        }
                                    }
                                    else
                                    {
                                        setCurrentItem(qtwitem);
                                        rowPosition = currentRow();
                                        columnPosition = currentColumn();
                                    }
                                    QTableWidget::mousePressEvent(e);
                                        
                                    QAction* action = menu->exec(e->globalPos());
                                    if (action == actionAddColumn)
                                    {
                                        addSkill(columnPosition);
                                    }
                                    else if (action == actionRemoveColumn)
                                    {
                                        if (qtwitem != NULL)
                                        {
                                            removeSkill(columnPosition);
                                        }
                                    }
                                    else if (action == actionEditColumn)
                                    {
                                        if (qtwitem != NULL)
                                        {
                                            editSkill(columnPosition);
                                        }
                                    }
                                    else if (action == actionAddRow)
                                    {
                                        addCharacter(rowPosition);
                                    }
                                    else if (action == actionRemoveRow)
                                    {
                                        if (qtwitem != NULL)
                                        {
                                            removeCharacter(rowPosition);
                                        }
                                    }
                                    else if (action == actionEditRow)
                                    {
                                        if (qtwitem != NULL)
                                        {
                                            editCharacter(rowPosition);
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
        }
        // setting values
        k = 0;
        for (Character::SkillIterator itSkill = (*it).begin(); itSkill != (*it).end() && k<i; itSkill++)
        {
            setItem(j,k,new QTableWidgetItem((*itSkill).c_str()));
            k++;
        } 
        for (;k<i;k++)
        {
            setItem(j,k,new QTableWidgetItem("0"));
        }
        j++;
    }
    resizeRowsToContents();
    resizeColumnsToContents();
}

void QCustomTableWidget::onCellChanged(int row, int column)
{
    if (pCharacters != NULL)
    {
        Character &charact = (*pCharacters)[row];
        for (int i=charact.skillNumber(); i<column+1; i++)
        {
            charact.addSkill("0");
        }
        charact.skill(column) = item(row,column)->text().toStdString();
    }
    resizeColumnToContents(column);
}

void QCustomTableWidget::onHHeaderClicked(int index, const QPoint &position)
{
    QAction *action = hMenu->exec(position);
    if (action == actionHAdd)
    {
        addSkill(index);
    }
    else if (action == actionHRemove)
    {
        removeSkill(index);
    }
    else if (action == actionHEdit)
    {
        editSkill(index);
    }
}

void QCustomTableWidget::onVHeaderClicked(int index, const QPoint &position)
{
    QAction *action = vMenu->exec(position);
    if (action == actionVAdd)
    {
        addCharacter(index);
    }
    else if (action == actionVRemove)
    {
        removeCharacter(index);
    }
    else if (action == actionVEdit)
    {
        editCharacter(index);
    }
}

void QCustomTableWidget::addCharacter(int index)
{
    if(pChangeCharacterDial->exec()==QDialog::Accepted)
    {
        // updating the CharacterList
        if (pCharacters != NULL)
        {
            Character character(pChangeCharacterDial->name().toStdString(),pChangeCharacterDial->playerName().toStdString());
            pCharacters->add(character,index+1);
        }

        // updating the display
        insertRow(index+1);
        int column_nb;
        column_nb = columnCount();
        for (int i = 0; i < column_nb; i++)
        {
            QTableWidgetItem *row1 = new QTableWidgetItem("0");
            setItem(index+1,i,row1);
        }
        QTableWidgetItem *rowHeaderItem = verticalHeaderItem(index+1);
        if (rowHeaderItem != NULL)
        {
            rowHeaderItem->setText(pChangeCharacterDial->name()+"\n"+pChangeCharacterDial->playerName());
        }
        else
        {
            rowHeaderItem = new QTableWidgetItem(pChangeCharacterDial->name()+"\n"+pChangeCharacterDial->playerName());
            setVerticalHeaderItem(index+1, rowHeaderItem);
        }
        resizeRowsToContents();
    }
}

void QCustomTableWidget::addSkill(int index)
{
    if(pChangeSkillDial->exec()==QDialog::Accepted)
    {
        // modifying the skill/character Lists
        if (pSkills != NULL)
        {
            pSkills->add(pChangeSkillDial->text().toStdString(),index+1);
        }
        if (pCharacters != NULL)
        {
            for (CharacterList::iterator it=pCharacters->begin(); it != pCharacters->end(); it++)
            {
                if ((unsigned int)index+1 < it->skillNumber())
                {
                    // adding a skill
                    it->addSkill("0",index+1);
                }
            } 
        }

        // updating the display
        insertColumn(index+1);
        int row_nb;
        row_nb = rowCount();
        for (int i = 0; i < row_nb; i++)
        {
            QTableWidgetItem *col1 = new QTableWidgetItem("0");
            setItem(i,index+1,col1);
        }
        QTableWidgetItem *columnHeaderItem = horizontalHeaderItem(index+1);
        if (columnHeaderItem != NULL)
        {
            columnHeaderItem->setText(pChangeSkillDial->text());
        }
        else
        {
            columnHeaderItem = new QTableWidgetItem(pChangeSkillDial->text());
            setHorizontalHeaderItem(index+1, columnHeaderItem);
        }
    }
    resizeColumnsToContents();
}

void QCustomTableWidget::removeCharacter(int index)
{
    removeRow(index);
    // updating the CharacterList
    if (pCharacters != NULL)
    {
        pCharacters->remove(index);
    }
}

void QCustomTableWidget::removeSkill(int index)
{
    removeColumn(index);
    // updating the she skill/character Lists
    if (pSkills != NULL)
    {
        pSkills->remove(index);
    }
    if (pCharacters != NULL)
    {
        for (CharacterList::iterator it=pCharacters->begin(); it != pCharacters->end(); it++)
        {
            if ((unsigned int)index < it->skillNumber())
            {
                it->removeSkill(index);
            }
        } 
    }
}

void QCustomTableWidget::editCharacter(int index)
{
    if(pChangeCharacterDial->exec()==QDialog::Accepted)
    {
        QTableWidgetItem *rowHeaderItem = verticalHeaderItem ( index );
        if (rowHeaderItem != NULL)
        {
            rowHeaderItem->setText(pChangeCharacterDial->name()+"\n"+pChangeCharacterDial->playerName());
        }
        else
        {
            rowHeaderItem = new QTableWidgetItem(pChangeCharacterDial->name()+"\n"+pChangeCharacterDial->playerName());
            setVerticalHeaderItem(index, rowHeaderItem);
        }
        // updating the CharacterList
        if (pCharacters != NULL)
        {
            Character &charact = (*pCharacters)[index];
            charact.setName(pChangeCharacterDial->name().toStdString());
            charact.setPlayerName(pChangeCharacterDial->playerName().toStdString());
        }
    }
    resizeRowToContents(index);
}

void QCustomTableWidget::editSkill(int index)
{
    if(pChangeSkillDial->exec()==QDialog::Accepted)
    {
        QTableWidgetItem *columnHeaderItem = horizontalHeaderItem(index);
        if (columnHeaderItem != NULL)
        {
            columnHeaderItem->setText(pChangeSkillDial->text());
        }
        else
        {
            columnHeaderItem = new QTableWidgetItem(pChangeSkillDial->text());
            setHorizontalHeaderItem(index, columnHeaderItem);
        }
        // updating the SkillList
        if (pSkills != NULL)
        {
            (*pSkills)[index] = pChangeSkillDial->text().toStdString();
        }
    }
    resizeColumnToContents(index);
}
