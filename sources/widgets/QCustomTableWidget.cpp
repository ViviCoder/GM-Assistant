/*************************************************************************
* Copyright © 2011-2012 Vincent Prat & Simon Nicolas
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
#include "CharacterModification.h"

QCustomTableWidget::QCustomTableWidget(QWidget *parent): QTableWidget(parent), menu(new QMenu(this)), hMenu(new QMenu(this)), vMenu(new QMenu(this)), pChangeSkillDial(new ChangeSkillDialog(this)), pChangeCharacterDial(new ChangeCharacterDialog(this)), pSkills(0), pCharacters(0), bEditing(false)
{
    // popup menu
    // skills
    QMenu *menuColumn = menu->addMenu(QApplication::translate("customTable","&Skill",0));
    actionAddColumn = new QAction(QIcon(":/data/images/add.svg"),QApplication::translate("customTable","&Add",0),this);
    actionAddColumn->setIconVisibleInMenu(true);
    actionAddColumn->setStatusTip(QApplication::translate("customTable","Add a new skill",0));
    actionRemoveColumn = new QAction(QIcon(":/data/images/remove.svg"),QApplication::translate("customTable","&Remove",0),this);
    actionRemoveColumn->setIconVisibleInMenu(true);
    actionRemoveColumn->setStatusTip(QApplication::translate("customTable","Remove the skill",0));
    actionEditColumn = new QAction(QIcon(":/data/images/son.svg"),QApplication::translate("customTable","&Edit",0),this);
    actionEditColumn->setIconVisibleInMenu(true);
    actionEditColumn->setStatusTip(QApplication::translate("customTable","Edit the skill",0));
    menuColumn->addAction(actionAddColumn);
    menuColumn->addAction(actionRemoveColumn);
    menuColumn->addAction(actionEditColumn);
    // characters
    QMenu *menuRow = menu->addMenu(QApplication::translate("customTable","&Character",0));
    actionAddRow = new QAction(QIcon(":/data/images/add.svg"),QApplication::translate("customTable","&Add",0),this);
    actionAddRow->setIconVisibleInMenu(true);
    actionAddRow->setStatusTip(QApplication::translate("customTable","Add a new character",0));
    actionRemoveRow = new QAction(QIcon(":/data/images/remove.svg"),QApplication::translate("customTable","&Remove",0),this);
    actionRemoveRow->setIconVisibleInMenu(true);
    actionRemoveRow->setStatusTip(QApplication::translate("customTable","Remove the character",0));
    actionEditRow = new QAction(QIcon(":/data/images/son.svg"),QApplication::translate("customTable","&Edit",0),this);
    actionEditRow->setIconVisibleInMenu(true);
    actionEditRow->setStatusTip(QApplication::translate("customTable","Edit the character",0));
    menuRow->addAction(actionAddRow);
    menuRow->addAction(actionRemoveRow);
    menuRow->addAction(actionEditRow);

    // popup menu for vertical header
    actionVAdd = new QAction(QIcon(":/data/images/add.svg"),QApplication::translate("customTable","&Add",0),this);
    actionVAdd->setIconVisibleInMenu(true);
    actionVAdd->setStatusTip(QApplication::translate("customTable","Add a new character",0));
    actionVRemove = new QAction(QIcon(":/data/images/remove.svg"),QApplication::translate("customTable","&Remove",0),this);
    actionVRemove->setIconVisibleInMenu(true);
    actionVRemove->setStatusTip(QApplication::translate("customTable","Remove the character",0));
    actionVEdit = new QAction(QIcon(":/data/images/son.svg"),QApplication::translate("customTable","&Edit",0),this);
    actionVEdit->setIconVisibleInMenu(true);
    actionVEdit->setStatusTip(QApplication::translate("customTable","Edit the character",0));
    vMenu->addAction(actionVAdd);
    vMenu->addAction(actionVRemove);
    vMenu->addAction(actionVEdit);

    // popup menu for horizontal header
    actionHAdd = new QAction(QIcon(":/data/images/add.svg"),QApplication::translate("customTable","&Add",0),this);
    actionHAdd->setIconVisibleInMenu(true);
    actionHAdd->setStatusTip(QApplication::translate("customTable","Add a new skill",0));
    actionHRemove = new QAction(QIcon(":/data/images/remove.svg"),QApplication::translate("customTable","&Remove",0),this);
    actionHRemove->setIconVisibleInMenu(true);
    actionHRemove->setStatusTip(QApplication::translate("customTable","Remove the skill",0));
    actionHEdit = new QAction(QIcon(":/data/images/son.svg"),QApplication::translate("customTable","&Edit",0),this);
    actionHEdit->setIconVisibleInMenu(true);
    actionHEdit->setStatusTip(QApplication::translate("customTable","Edit the skill",0));
    hMenu->addAction(actionHAdd);
    hMenu->addAction(actionHRemove);
    hMenu->addAction(actionHEdit);

    // headers
    setHorizontalHeader(new QCustomHeaderView(Qt::Horizontal,this));
    setVerticalHeader(new QCustomHeaderView(Qt::Vertical,this));

    // connection of signals
    connect(this,SIGNAL(cellChanged(int,int)),this,SLOT(onCellChanged(int,int)));
    connect(dynamic_cast<QCustomHeaderView*>(horizontalHeader()),SIGNAL(rightClicked(int, const QPoint&)),this,SLOT(onHHeaderClicked(int, const QPoint&)));
    connect(dynamic_cast<QCustomHeaderView*>(verticalHeader()),SIGNAL(rightClicked(int, const QPoint&)),this,SLOT(onVHeaderClicked(int, const QPoint&)));
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(on_itemSelectionChanged()));
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
                                    if (qtwitem && !qtwitem->isSelected())
                                    {
                                        QTableWidget::mousePressEvent(e);
                                    }
                                    else
                                    {
                                        setCurrentItem(0);
                                    }
                                    break;
                                }
        case Qt::RightButton:   {
                                    if (qtwitem)
                                    {
                                        setCurrentItem(qtwitem);
                                        rowPosition = currentRow();
                                        columnPosition = currentColumn();
                                    }
                                    else
                                    {
                                        rowPosition = rowCount()-1;
                                        columnPosition = columnCount()-1;
                                    }
                                    QTableWidget::mousePressEvent(e);
                                        
                                    bool null = (qtwitem != 0);
                                    actionRemoveColumn->setVisible(null);
                                    actionEditColumn->setVisible(null);
                                    actionRemoveRow->setVisible(null);
                                    actionEditRow->setVisible(null);
                                    QAction* action = menu->exec(e->globalPos());
                                    if (action == actionAddColumn)
                                    {
                                        addSkill(columnPosition);
                                    }
                                    else if (action == actionRemoveColumn)
                                    {
                                        if (qtwitem)
                                        {
                                            removeSkill(columnPosition);
                                        }
                                    }
                                    else if (action == actionEditColumn)
                                    {
                                        if (qtwitem)
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
                                        if (qtwitem)
                                        {
                                            removeCharacter(rowPosition);
                                        }
                                    }
                                    else if (action == actionEditRow)
                                    {
                                        if (qtwitem)
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
    if (item)
    {
        switch (e->key())
        {
            case Qt::Key_F2:    if (!bEditing)
                                {
                                    bEditing = true;
                                }
                                break;
            case Qt::Key_Delete:    if (!bEditing)
                                    {
                                        item->setText("0");
                                    }
                                    break;
            case Qt::Key_Return:
            case Qt::Key_Enter:
            case Qt::Key_Escape:    bEditing = false;
                                    break;
            default:    QTableWidget::keyReleaseEvent(e); break; 
        }
    }
}

void QCustomTableWidget::setLists(SkillList *skills, CharacterList *characters)
{
    pSkills = skills;
    pCharacters = characters;
    updateDisplay();
}

void QCustomTableWidget::updateDisplay()
{
    clear();
    setColumnCount(0);
    setRowCount(0);
    int i=0;
    for (SkillList::iterator it = pSkills->begin(); it != pSkills->end(); it++)
    {
        insertColumn(i);
        setHorizontalHeaderItem(i,new QTableWidgetItem((*it).c_str()));
        i++;
    }
    int j=0,k;
    for (CharacterList::iterator it = pCharacters->begin(); it != pCharacters->end(); it++)
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
    if (pCharacters)
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
        if (pCharacters)
        {
            Character character(pChangeCharacterDial->name().toStdString(),pChangeCharacterDial->playerName().toStdString());
            pCharacters->add(character,index+1);
            emit modificationDone(new CharacterModification(pCharacters, new Character(character), index+1, true));
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
        if (rowHeaderItem)
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
        if (pSkills)
        {
            std::string skill = pChangeSkillDial->text().toStdString();
            pSkills->add(skill, index+1);
            emit modificationDone(new CharacterModification(pSkills, skill, pCharacters, index+1));
        }
        if (pCharacters)
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
        if (columnHeaderItem)
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
    if (pCharacters)
    {
        emit modificationDone(new CharacterModification(pCharacters, new Character((*pCharacters)[index]), index, false));
        pCharacters->remove(index);
    }
}

void QCustomTableWidget::removeSkill(int index)
{
    removeColumn(index);
    // updating the she skill/character Lists
    std::vector<std::string> values;
    if (pCharacters)
    {
        for (CharacterList::iterator it = pCharacters->begin(); it != pCharacters->end(); it++)
        {
            if ((unsigned int)index < it->skillNumber())
            {
                values.push_back(it->skill(index));
                it->removeSkill(index);
            }
        } 
    }
    if (pSkills)
    {
        if (pCharacters)
        {
            emit modificationDone(new CharacterModification(pSkills, (*pSkills)[index], pCharacters, values, index));
        }
        pSkills->remove(index);
    }
}

void QCustomTableWidget::editCharacter(int index)
{
    Character *character = &(*pCharacters)[index];
    if(pChangeCharacterDial->exec(character)==QDialog::Accepted)
    {
        QTableWidgetItem *rowHeaderItem = verticalHeaderItem(index);
        rowHeaderItem->setText(pChangeCharacterDial->name()+"\n"+pChangeCharacterDial->playerName());
        // updating the CharacterList
        if (pCharacters)
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
    QTableWidgetItem *columnHeaderItem = horizontalHeaderItem(index);
    if(pChangeSkillDial->exec(columnHeaderItem->text())==QDialog::Accepted)
    {
        QTableWidgetItem *columnHeaderItem = horizontalHeaderItem(index);
        columnHeaderItem->setText(pChangeSkillDial->text());
        // updating the SkillList
        if (pSkills)
        {
            (*pSkills)[index] = pChangeSkillDial->text().toStdString();
        }
    }
    resizeColumnToContents(index);
}

void QCustomTableWidget::on_itemSelectionChanged()
{
    bEditing = false;
}

void QCustomTableWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    QTableWidgetItem *item = currentItem();
    if (item)
    {
        editItem(item);
        bEditing = true;
    }
}
