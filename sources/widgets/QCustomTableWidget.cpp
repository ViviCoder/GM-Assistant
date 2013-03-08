/*************************************************************************
* Copyright © 2011-2013 Vincent Prat & Simon Nicolas
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
#include <QScrollBar>

QCustomTableWidget::QCustomTableWidget(QWidget *parent): QTableWidget(parent), menu(new QMenu(this)), hMenu(new QMenu(this)), vMenu(new QMenu(this)), pChangeSkillDial(new ChangeSkillDialog(this)), pChangeCharacterDial(new ChangeCharacterDialog(this)), pSkills(0), pCharacters(0), bEditing(false), bUpdate(false), iCreatedCells(0)
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
    actionEditColumn = new QAction(QIcon(":/data/images/pencil.svg"),QApplication::translate("customTable","&Edit",0),this);
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
    actionEditRow = new QAction(QIcon(":/data/images/pencil.svg"),QApplication::translate("customTable","&Edit",0),this);
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
    actionVEdit = new QAction(QIcon(":/data/images/pencil.svg"),QApplication::translate("customTable","&Edit",0),this);
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
    actionHEdit = new QAction(QIcon(":/data/images/pencil.svg"),QApplication::translate("customTable","&Edit",0),this);
    actionHEdit->setIconVisibleInMenu(true);
    actionHEdit->setStatusTip(QApplication::translate("customTable","Edit the skill",0));
    hMenu->addAction(actionHAdd);
    hMenu->addAction(actionHRemove);
    hMenu->addAction(actionHEdit);

    // headers
    setHorizontalHeader(new QCustomHeaderView(Qt::Horizontal,this));
    setVerticalHeader(new QCustomHeaderView(Qt::Vertical,this));

    // connection of signals
    QCustomHeaderView *header = dynamic_cast<QCustomHeaderView*>(horizontalHeader());
    connect(header, SIGNAL(rightClicked(int, const QPoint&)), this, SLOT(onHHeaderClicked(int, const QPoint&)));
    connect(header, SIGNAL(sectionMoved(int, int, int)), this, SLOT(onHHeaderMoved(int, int, int)));
    header = dynamic_cast<QCustomHeaderView*>(verticalHeader());
    connect(header, SIGNAL(rightClicked(int, const QPoint&)), this, SLOT(onVHeaderClicked(int, const QPoint&)));
    connect(header, SIGNAL(sectionMoved(int, int, int)), this, SLOT(onVHeaderMoved(int, int, int)));
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(on_itemSelectionChanged()));
    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(onCellChanged(int,int)));
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

void QCustomTableWidget::updateDisplay(int row, int column)
{
    // storing the scroll state
    QScrollBar *hbar = horizontalScrollBar();
    QScrollBar *vbar = verticalScrollBar();
    int x = hbar->value();
    int y = vbar->value();
    // iterating over characters and skills to populate the table
    bUpdate = true;
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
    bUpdate = false;
    resizeRowsToContents();
    resizeColumnsToContents();
    if (row > -1 || column > -1)
    {
        // restoring the scroll state
        hbar->setValue(x);
        vbar->setValue(y);
        scrollTo(row, column);
    }
}

void QCustomTableWidget::onCellChanged(int logicalRow, int logicalColumn)
{
    // visual indices
    int row = visualRow(logicalRow);
    int column = visualColumn(logicalColumn);
    if (pCharacters)
    {
        Character &charact = (*pCharacters)[row];
        for (int i=charact.skillNumber(); i<column+1; i++)
        {
            charact.addSkill("0");
        }
        std::string value = charact.skill(column);
        std::string newValue = item(logicalRow, logicalColumn)->text().toStdString();
        charact.skill(column) = newValue;
        if (iCreatedCells)
        {
            // we ignore the newly created cells
            iCreatedCells--;
        }
        else if (!bUpdate)
        {
            // if it is a real modification, we register it
            emit modificationDone(new CharacterModification(pCharacters, value, newValue, row, column));
        }
    }
    resizeColumnsToContents();
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
        int column_nb = columnCount();
        // created cells
        iCreatedCells = column_nb;
        insertRow(index+1);
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
        scrollTo(index+1, -1);
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
        int row_nb = rowCount();
        // created cells
        iCreatedCells = row_nb;
        insertColumn(index+1);
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
        resizeColumnsToContents();
        scrollTo(-1, index+1);
    }
}

void QCustomTableWidget::removeCharacter(int index)
{
    removeRow(logicalRow(index));
    // updating the CharacterList
    if (pCharacters)
    {
        emit modificationDone(new CharacterModification(pCharacters, new Character((*pCharacters)[index]), index, false));
        pCharacters->remove(index);
    }
}

void QCustomTableWidget::removeSkill(int index)
{
    removeColumn(logicalColumn(index));
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
    scrollTo(index, -1);
    if (pCharacters)
    {
        Character &character = (*pCharacters)[index];
        if(pChangeCharacterDial->exec(&character)==QDialog::Accepted)
        {
            // updating the CharacterList
            std::string name = character.name();
            std::string playerName = character.playerName();
            character.setName(pChangeCharacterDial->name().toStdString());
            character.setPlayerName(pChangeCharacterDial->playerName().toStdString());
            emit modificationDone(new CharacterModification(pCharacters, name, playerName, character.name(), character.playerName(), index));
            index = logicalRow(index);
            QTableWidgetItem *rowHeaderItem = verticalHeaderItem(index);
            rowHeaderItem->setText(pChangeCharacterDial->name()+"\n"+pChangeCharacterDial->playerName());
        }
        resizeRowToContents(index);
        scrollTo(index, -1);
    }
}

void QCustomTableWidget::editSkill(int index)
{
    scrollTo(-1, index);
    QTableWidgetItem *columnHeaderItem = horizontalHeaderItem(index);
    if(pChangeSkillDial->exec(columnHeaderItem->text())==QDialog::Accepted)
    {
        // updating the SkillList
        if (pSkills)
        {
            std::string newSkill = pChangeSkillDial->text().toStdString();
            emit modificationDone(new CharacterModification(pSkills, (*pSkills)[index], newSkill, index));
            (*pSkills)[index] = newSkill;
        }
        index = logicalColumn(index);
        QTableWidgetItem *columnHeaderItem = horizontalHeaderItem(index);
        columnHeaderItem->setText(pChangeSkillDial->text());
        resizeColumnToContents(index);
        scrollTo(-1, index);
    }
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

void QCustomTableWidget::onHHeaderMoved(int, int oldColumn, int newColumn)
{
    if (pSkills)
    {
        if (pSkills->move(oldColumn, newColumn) && pCharacters)
        {
            emit modificationDone(new CharacterModification(pSkills, pCharacters, oldColumn, newColumn));
        }   
    }
    if (pCharacters)
    {
        for (CharacterList::iterator it = pCharacters->begin(); it != pCharacters->end(); it++)
        {
            it->moveSkill(oldColumn, newColumn);
        }
    }
}

void QCustomTableWidget::onVHeaderMoved(int, int oldRow, int newRow)
{
    if (pCharacters)
    {
        if (pCharacters->move(oldRow, newRow))
        {
            emit modificationDone(new CharacterModification(pCharacters, oldRow, newRow));
        }
    }
}

int QCustomTableWidget::logicalRow(int visualRow)
{
    return verticalHeader()->logicalIndex(visualRow);
}

int QCustomTableWidget::logicalColumn(int visualColumn)
{
    return horizontalHeader()->logicalIndex(visualColumn);
}

void QCustomTableWidget::scrollTo(int row, int column)
{
    repaint();
    int row_count = rowCount(), column_count = columnCount();
    if (row > -1 && row < row_count)
    {
        int y = rowViewportPosition(row);
        QScrollBar *bar = verticalScrollBar();
        if (y < 0)
        {
            bar->setValue(bar->value()+y);
        }
        else
        {
            int dy = y + rowHeight(row) - viewport()->height();
            if (columnViewportPosition(column_count-1) + columnWidth(column_count-1) > viewport()->width())
            {
                dy += horizontalScrollBar()->height() + 3;
            }
            if (dy > 0)
            {
                int value = bar->value() + dy;
                if (value > bar->maximum())
                {
                    bar->setMaximum(value);
                }
                bar->setValue(value);
            }
        }
    }   
    if (column > -1 && column < column_count)
    {
        int x = columnViewportPosition(column);
        QScrollBar *bar = horizontalScrollBar();
        if (x < 0)
        {
            bar->setValue(bar->value()+x);
        }
        else
        {
            int dx = x + columnWidth(column) - viewport()->width();
            if (dx > 0)
            {
                int value = bar->value() + dx;
                if (value > bar->maximum())
                {
                    bar->setMaximum(value);
                }
                bar->setValue(value);
            }
        }
    }   
}

void QCustomTableWidget::updateModification(CharacterModification *modification, bool undo)
{
    // selection of the cell to focus on
    int row = -1;
    int column = -1;
    switch (modification->editionType())
    {
        case CharacterModification::etCharacter:    if (undo)
                                                    {
                                                        switch (modification->action())
                                                        {
                                                            case Modification::aAddition:   row = modification->index()-1;
                                                                                            break;
                                                            default:    row = modification->index();
                                                        }
                                                    }
                                                    else
                                                    {
                                                        switch (modification->action())
                                                        {
                                                            case Modification::aDeletion:   row = modification->index()-1;
                                                                                            break;
                                                            case Modification::aMovement:   row = modification->newIndex();
                                                                                            break;
                                                            default:    row = modification->index();
                                                        }
                                                    }
                                                    break;
        case CharacterModification::etSkill:    if (undo)
                                                {
                                                    switch (modification->action())
                                                    {
                                                        case Modification::aAddition:   column = modification->index()-1;
                                                                                        break;
                                                        default:    column = modification->index();
                                                    }
                                                }
                                                else
                                                {
                                                    switch (modification->action())
                                                    {
                                                        case Modification::aDeletion:   column = modification->index()-1;
                                                                                        break;
                                                        case Modification::aMovement:   column = modification->newIndex();
                                                                                        break;
                                                        default:    column = modification->index();
                                                    }
                                                }
                                                break;
        case CharacterModification::etValue:    row = modification->index();
                                                column = modification->newIndex();
                                                break;
    }
    updateDisplay(row, column);
}
