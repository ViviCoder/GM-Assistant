/*************************************************************************
* Copyright © 2011-2020 Vincent Prat & Simon Nicolas
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
#include "ChangePropertyDialog.h"
#include <QApplication>
#include <QScrollBar>
#include <QToolTip>

QCustomTableWidget::QCustomTableWidget(QWidget *parent): QTableWidget(parent), menu(new QMenu(this)), hMenu(new QMenu(this)), vMenu(new QMenu(this)), pChangePropertyDial(new ChangePropertyDialog(this)), pChangeCharacterDial(new ChangeCharacterDialog(this)), pProperties(0), pCharacters(0), bEditing(false), bUpdate(false), iCreatedCells(0), pVHeader(new QCustomHeaderView(Qt::Vertical,this)), eSelected(QAbstractItemView::SelectItems)
{
    // property menu
    actionAddColumn = new QAction(this);
    actionAddColumn->setIcon(QIcon(":/data/images/add.svg"));
    actionRemoveColumn = new QAction(this);
    actionRemoveColumn->setIcon(QIcon(":/data/images/remove.svg"));
    actionEditColumn = new QAction(this);
    actionEditColumn->setIcon(QIcon(":/data/images/pencil.svg"));
    hMenu->addAction(actionAddColumn);
    hMenu->addAction(actionRemoveColumn);
    hMenu->addAction(actionEditColumn);
    // character menu
    actionAddRow = new QAction(this);
    actionAddRow->setIcon(QIcon(":/data/images/add.svg"));
    actionRemoveRow = new QAction(this);
    actionRemoveRow->setIcon(QIcon(":/data/images/remove.svg"));
    actionEditRow = new QAction(this);
    actionEditRow->setIcon(QIcon(":/data/images/pencil.svg"));
    vMenu->addAction(actionAddRow);
    vMenu->addAction(actionRemoveRow);
    vMenu->addAction(actionEditRow);
    // main menu
    menu->addMenu(hMenu);
    menu->addMenu(vMenu);

    // headers
    setVerticalHeader(pVHeader);
    QCustomHeaderView *header = new QCustomHeaderView(Qt::Horizontal,this);
    setHorizontalHeader(header);

    // connections with the horizontal header
    connect(header, SIGNAL(clicked(int, Qt::MouseButton, const QPoint&)), this, SLOT(onHHeaderClicked(int, Qt::MouseButton, const QPoint&)));
    connect(header, SIGNAL(sectionMoved(int, int, int)), this, SLOT(onHHeaderMoved(int, int, int)));
    // connections with the vertical header
    connect(pVHeader, SIGNAL(clicked(int, Qt::MouseButton, const QPoint&)), this, SLOT(onVHeaderClicked(int, Qt::MouseButton, const QPoint&)));
    connect(pVHeader, SIGNAL(sectionMoved(int, int, int)), this, SLOT(onVHeaderMoved(int, int, int)));
    connect(pVHeader, SIGNAL(sectionDoubleClicked(int)), this, SLOT(onCharacterDoubleClicked(int)));
    connect(pVHeader, SIGNAL(toolTipRequested(int, const QPoint&)), this, SLOT(onVHeaderToolTipRequested(int, const QPoint&)));
    // self-connections
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(on_itemSelectionChanged()));
    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(onCellChanged(int,int)));

    // sets the text of the menus and a few other things
    retranslate();
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
                                        rowPosition = visualRow(currentRow());
                                        columnPosition = visualColumn(currentColumn());
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
                                        addProperty(columnPosition);
                                    }
                                    else if (action == actionRemoveColumn)
                                    {
                                        if (qtwitem)
                                        {
                                            removeProperty(columnPosition);
                                        }
                                    }
                                    else if (action == actionEditColumn)
                                    {
                                        if (qtwitem)
                                        {
                                            editProperty(columnPosition);
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
    int row = visualRow(currentRow());
    int column = visualColumn(currentColumn());
    Qt::KeyboardModifiers modifs = e->modifiers();
    switch (e->key())
    {
        case Qt::Key_F2:    if (!bEditing)
                            {
                                if (modifs == Qt::ControlModifier)
                                {
                                    if (eSelected != QAbstractItemView::SelectRows)
                                    {
                                        editProperty(column);
                                    }
                                }
                                else if (modifs == (Qt::ControlModifier | Qt::ShiftModifier))
                                {
                                    if (eSelected != QAbstractItemView::SelectColumns)
                                    {
                                        editCharacter(row);
                                    }
                                }
                                else if (item && eSelected == QAbstractItemView::SelectItems)
                                {
                                    bEditing = true;
                                    editItem(item);
                                } 
                            }
                            break;
        case Qt::Key_Delete:    if (!bEditing)
                                {
                                    if (modifs == Qt::ControlModifier)
                                    {
                                        if (eSelected != QAbstractItemView::SelectRows)
                                        {
                                            removeProperty(column);
                                        }
                                    }
                                    else if (modifs == (Qt::ControlModifier | Qt::ShiftModifier))
                                    {
                                        if (eSelected != QAbstractItemView::SelectColumns)
                                        {
                                            removeCharacter(row);
                                        }
                                    }
                                    else if (item && eSelected == QAbstractItemView::SelectItems)
                                    {
                                        item->setText("0");
                                    }
                                }
                                break;
        case Qt::Key_Insert:    if (!bEditing)
                                {
                                    if (modifs == Qt::ControlModifier)
                                    {
                                        if (eSelected != QAbstractItemView::SelectRows)
                                        {
                                            addProperty(column);
                                        }
                                    }
                                    else if (modifs == (Qt::ControlModifier | Qt::ShiftModifier))
                                    {
                                        if (eSelected != QAbstractItemView::SelectColumns)
                                        {
                                            addCharacter(row);
                                        }
                                    }
                                    else
                                    {
                                        QTableWidget::keyReleaseEvent(e);
                                    }
                                }
                                break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Escape:    bEditing = false;
                                break;
        default:    QTableWidget::keyReleaseEvent(e); break; 
    }
}

void QCustomTableWidget::setLists(PropertyList *properties, CharacterList *characters)
{
    pProperties = properties;
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
    // iterating over characters and properties to populate the table
    bUpdate = true;
    clear();
    setColumnCount(0);
    setRowCount(0);
    int i=0;
    for (PropertyList::iterator it = pProperties->begin(); it != pProperties->end(); it++)
    {
        insertColumn(i);
        setHorizontalHeaderItem(i,new QTableWidgetItem((*it).c_str()));
        i++;
    }
    int j=0,k;
    for (CharacterList::iterator it = pCharacters->begin(); it != pCharacters->end(); it++)
    {
        insertRow(j);
        Character *character = (*it);
        // open the associated note if necessary
        Note *note = character->note();
        if (note->visible())
        {
            emit noteToOpen(note);
        }
        // create the header
        setVerticalHeaderItem(j, new QTableWidgetItem(headerText(character->name().c_str(), character->shortDescription().c_str())));
        // setting values
        k = 0;
        for (Character::PropertyIterator itProperty = character->begin(); itProperty != character->end() && k<i; itProperty++)
        {
            setItem(j,k,new QTableWidgetItem((*itProperty).c_str()));
            k++;
        } 
        for (;k<i;k++)
        {
            setItem(j,k,new QTableWidgetItem("0"));
        }
        j++;
    }
    bUpdate = false;
    if (row > -1 || column > -1)
    {
        // restoring the scroll state
        hbar->setValue(x);
        vbar->setValue(y);
        scrollTo(row, column);
        // setting the current cell, row or column
        QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::ClearAndSelect;
        if (row == -1)
        {
            flags |= QItemSelectionModel::Columns;
            row = 0;
        }
        if (column == -1)
        {
            flags |= QItemSelectionModel::Rows;
            column = 0;
        }
        setCurrentCell(row, column, flags);
    }
}

void QCustomTableWidget::onCellChanged(int logicalRow, int logicalColumn)
{
    // visual indices
    int row = visualRow(logicalRow);
    int column = visualColumn(logicalColumn);
    if (pCharacters)
    {
        Character *charact = (*pCharacters)[row];
        for (int i=charact->propertyNumber(); i<column+1; i++)
        {
            charact->addProperty("0");
        }
        std::string value = charact->property(column);
        std::string newValue = item(logicalRow, logicalColumn)->text().toStdString();
        charact->property(column) = newValue;
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
}

void QCustomTableWidget::onHHeaderClicked(int index, Qt::MouseButton button, const QPoint &position)
{
    if (button == Qt::LeftButton)
    {
        // select the column
        setCurrentCell(0, index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Columns);
    }
    else if (button == Qt::RightButton)
    {
        index = visualColumn(index);
        // show the popup menu
        bool null = (index != -1);
        actionRemoveColumn->setVisible(null);
        actionEditColumn->setVisible(null);
        QAction *action = hMenu->exec(position);
        if (action == actionAddColumn)
        {
            addProperty(index);
        }
        else if (action == actionRemoveColumn)
        {
            removeProperty(index);
        }
        else if (action == actionEditColumn)
        {
            editProperty(index);
        }
    }
}

void QCustomTableWidget::onVHeaderClicked(int index, Qt::MouseButton button, const QPoint &position)
{
    if (button == Qt::LeftButton)
    {
        // select the row
        setCurrentCell(index, 0, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
    else if (button == Qt::RightButton)
    {
        index = visualRow(index);
        // show the popup menu
        bool null = (index != -1);
        actionRemoveRow->setVisible(null);
        actionEditRow->setVisible(null);
        QAction *action = vMenu->exec(position);
        if (action == actionAddRow)
        {
            addCharacter(index);
        }
        else if (action == actionRemoveRow)
        {
            removeCharacter(index);
        }
        else if (action == actionEditRow)
        {
            editCharacter(index);
        }
    }
}

void QCustomTableWidget::addCharacter(int index)
{
    if (index < 0)
    {
        // if no row is selected, we insert it at the end
        index += rowCount();
    }
    if(pChangeCharacterDial->exec()==QDialog::Accepted)
    {
        // updating the CharacterList
        if (pCharacters)
        {
            Character *character = new Character(pChangeCharacterDial->name().toStdString(), pChangeCharacterDial->shortDescription().toStdString());
            pCharacters->add(character,index+1);
            emit modificationDone(new CharacterModification(pCharacters, index+1));
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
            rowHeaderItem->setText(headerText(pChangeCharacterDial->name(), pChangeCharacterDial->shortDescription()));
        }
        else
        {
            rowHeaderItem = new QTableWidgetItem(headerText(pChangeCharacterDial->name(), pChangeCharacterDial->shortDescription()));
            setVerticalHeaderItem(index+1, rowHeaderItem);
        }
        scrollTo(index+1, -1);
        setCurrentCell(index+1, 0, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
}

void QCustomTableWidget::addProperty(int index)
{
    if (index < 0)
    {
        // if no column is selected, we insert it at the end
        index += columnCount();
    }
    if(pChangePropertyDial->exec()==QDialog::Accepted)
    {
        // modifying the property/character Lists
        if (pProperties)
        {
            std::string property = pChangePropertyDial->text().toStdString();
            pProperties->add(property, index+1);
            emit modificationDone(new CharacterModification(pProperties, property, pCharacters, index+1));
        }
        if (pCharacters)
        {
            for (CharacterList::iterator it=pCharacters->begin(); it != pCharacters->end(); it++)
            {
                Character *character = *it;
                if ((unsigned int)index+1 < character->propertyNumber())
                {
                    // adding a property
                    character->addProperty("0",index+1);
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
            columnHeaderItem->setText(pChangePropertyDial->text());
        }
        else
        {
            columnHeaderItem = new QTableWidgetItem(pChangePropertyDial->text());
            setHorizontalHeaderItem(index+1, columnHeaderItem);
        }
        scrollTo(-1, index+1);
        setCurrentCell(0, index+1, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Columns);
    }
}

void QCustomTableWidget::removeCharacter(int index)
{
    if (index != -1)
    {
        removeRow(logicalRow(index));
        // updating the CharacterList
        if (pCharacters)
        {
            Character *character = (*pCharacters)[index];
            emit modificationDone(new CharacterModification(pCharacters, character, index));
            emit noteToDelete(character->note());
            pCharacters->remove(index);
        }
        if (index == rowCount())
        {
            index--;
        }
        setCurrentCell(index, 0, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
}

void QCustomTableWidget::removeProperty(int index)
{
    if (index != -1)
    {
        removeColumn(logicalColumn(index));
        // updating the she property/character Lists
        std::vector<std::string> values;
        if (pCharacters)
        {
            for (CharacterList::iterator it = pCharacters->begin(); it != pCharacters->end(); it++)
            {
                Character *character = *it;
                if ((unsigned int)index < character->propertyNumber())
                {
                    values.push_back(character->property(index));
                    character->removeProperty(index);
                }
            } 
        }
        if (pProperties)
        {
            if (pCharacters)
            {
                emit modificationDone(new CharacterModification(pProperties, (*pProperties)[index], pCharacters, values, index));
            }
            pProperties->remove(index);
        }
        if (index == columnCount())
        {
            index--;
        }
        setCurrentCell(0, index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Columns);
    }
}

void QCustomTableWidget::editCharacter(int index)
{
    if (index != -1)
    {
        scrollTo(index, -1);
        if (pCharacters)
        {
            Character *character = (*pCharacters)[index];
            if(pChangeCharacterDial->exec(character)==QDialog::Accepted)
            {
                // updating the CharacterList
                std::string name = character->name();
                std::string shortDescription = character->shortDescription();
                character->setName(pChangeCharacterDial->name().toStdString());
                character->setShortDescription(pChangeCharacterDial->shortDescription().toStdString());
                emit modificationDone(new CharacterModification(pCharacters, name, shortDescription, character->name(), character->shortDescription(), index));
                QTableWidgetItem *rowHeaderItem = verticalHeaderItem(logicalRow(index));
                rowHeaderItem->setText(headerText(pChangeCharacterDial->name(), pChangeCharacterDial->shortDescription()));
            }
            scrollTo(index, -1);
        }
    }
}

void QCustomTableWidget::editProperty(int index)
{
    if (index != -1)
    {
        scrollTo(-1, index);
        int logIndex = logicalColumn(index);
        QTableWidgetItem *columnHeaderItem = horizontalHeaderItem(logIndex);
        if(pChangePropertyDial->exec(columnHeaderItem->text())==QDialog::Accepted)
        {
            // updating the PropertyList
            if (pProperties)
            {
                std::string newProperty = pChangePropertyDial->text().toStdString();
                emit modificationDone(new CharacterModification(pProperties, (*pProperties)[index], newProperty, index));
                (*pProperties)[index] = newProperty;
            }
            columnHeaderItem->setText(pChangePropertyDial->text());
            scrollTo(-1, index);
        }
    }
}

void QCustomTableWidget::on_itemSelectionChanged()
{
    bEditing = false;
    // by default: an item is selected
    eSelected = QAbstractItemView::SelectItems;
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();
    if (!ranges.isEmpty())
    {
        QTableWidgetSelectionRange range = ranges.first();
        if (range.columnCount() > 1)
        {
            // a row is selected
            eSelected = QAbstractItemView::SelectRows;
        }
        else if (range.rowCount() > 1)
        {
            // a column is selected
            eSelected = QAbstractItemView::SelectColumns;
        }
    }
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
    if (pProperties)
    {
        if (pProperties->move(oldColumn, newColumn) && pCharacters)
        {
            emit modificationDone(new CharacterModification(pProperties, pCharacters, oldColumn, newColumn));
        }   
    }
    if (pCharacters)
    {
        for (CharacterList::iterator it = pCharacters->begin(); it != pCharacters->end(); it++)
        {
            (*it)->moveProperty(oldColumn, newColumn);
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
                                                                                            emit noteToDelete(modification->character()->note());
                                                                                            break;
                                                            default:    row = modification->index();
                                                        }
                                                    }
                                                    else
                                                    {
                                                        switch (modification->action())
                                                        {
                                                            case Modification::aDeletion:   row = modification->index()-1;
                                                                                            emit noteToDelete(modification->character()->note());
                                                                                            break;
                                                            case Modification::aMovement:   row = modification->newIndex();
                                                                                            break;
                                                            default:    row = modification->index();
                                                        }
                                                    }
                                                    break;
        case CharacterModification::etProperty:    if (undo)
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
    setFocus(Qt::OtherFocusReason);
}

void QCustomTableWidget::retranslate()
{
    hMenu->setTitle(QApplication::translate("customTable","&Property",0));
    actionAddColumn->setText(QApplication::translate("customTable","&Add",0));
    actionAddColumn->setStatusTip(QApplication::translate("customTable","Add a new property",0));
    actionAddColumn->setShortcut(QString("Ctrl+Ins"));
    actionRemoveColumn->setText(QApplication::translate("customTable","&Remove",0));
    actionRemoveColumn->setStatusTip(QApplication::translate("customTable","Remove the property",0));
    actionRemoveColumn->setShortcut(QString("Ctrl+Del"));
    actionEditColumn->setText(QApplication::translate("customTable","&Edit",0));
    actionEditColumn->setStatusTip(QApplication::translate("customTable","Edit the property",0));
    actionEditColumn->setShortcut(QString("Ctrl+F2"));
    vMenu->setTitle(QApplication::translate("customTable","&Character",0));
    actionAddRow->setText(QApplication::translate("customTable","&Add",0));
    actionAddRow->setStatusTip(QApplication::translate("customTable","Add a new character",0));
    actionAddRow->setShortcut(QString("Ctrl+Shift+Ins"));
    actionRemoveRow->setText(QApplication::translate("customTable","&Remove",0));
    actionRemoveRow->setStatusTip(QApplication::translate("customTable","Remove the character",0));
    actionRemoveRow->setShortcut(QString("Ctrl+Shift+Del"));
    actionEditRow->setText(QApplication::translate("customTable","&Edit",0));
    actionEditRow->setStatusTip(QApplication::translate("customTable","Edit the character",0));
    actionEditRow->setShortcut(QString("Ctrl+Shift+F2"));
    pVHeader->setStatusTip(QApplication::translate("customTable", "Double click to display the note associated with a character", 0));
}

void QCustomTableWidget::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        retranslate();
    }
}

QString QCustomTableWidget::headerText(const QString &name, const QString &description)
{
    QString result(name);
    if (!description.isEmpty())
    {
        result += "\n(" + description + ")";
    }
    return result;
}

int QCustomTableWidget::sizeHintForColumn(int column) const
{
    QStyleOptionViewItem option(viewOptions());
    int hint = 0;
    QAbstractItemDelegate *delegate = itemDelegate();
    QAbstractItemModel *itemModel = model();
    for(int row = 0; row < rowCount(); row++)
    {
        hint = qMax(hint, delegate->sizeHint(option, itemModel->index(row,column)).width());
    }
    if (showGrid())
    {
        hint++;
    }

    return hint;
}

void QCustomTableWidget::onCharacterDoubleClicked(int index)
{
    if (index != -1)
    {
        int row = visualRow(index);
        emit noteToOpen((*pCharacters)[row]->note());
    }
}

void QCustomTableWidget::onVHeaderToolTipRequested(int index, const QPoint &position)
{
    if (index != -1)
    {
        QToolTip::showText(position, (*pCharacters)[index]->note()->title().c_str());
    }
    else
    {
        QToolTip::hideText();
    }
}
