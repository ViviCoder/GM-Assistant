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

#ifndef HEADER_QCUSTOMTABLEWIDGET
#define HEADER_QCUSTOMTABLEWIDGET

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMouseEvent>
#include <QMenu>
#include "ChangeSkillDialog.h"
#include "ChangeCharacterDialog.h"
#include "SkillList.h"
#include "CharacterList.h"
#include "CharacterModification.h"

/*!
 * \brief Custom table widget
 *
 * This widget is designed to manipulate underlying CharacterList and SkillList
 */
class QCustomTableWidget: public QTableWidget
{
    Q_OBJECT

    private:
        QMenu *menu, *hMenu, *vMenu;
        QAction *actionAddColumn, *actionRemoveColumn, *actionAddRow, *actionRemoveRow, *actionEditRow, *actionEditColumn, *actionHAdd, *actionHRemove, *actionHEdit, *actionVAdd, *actionVRemove, *actionVEdit;
        ChangeSkillDialog *pChangeSkillDial;
        ChangeCharacterDialog *pChangeCharacterDial;
        //! Underlying list of skills
        SkillList *pSkills;
        //! Underlying list of characters
        CharacterList *pCharacters;
        //! Flag indicating if an item is currently being edited
        bool bEditing;
        //! Flag indicating if the widget is currently updating
        bool bUpdate;
        //! Number of recently created cells
        int iCreatedCells;
    protected:
        /*!
         * \brief Event raised when the table is clicked
         * \param e Mouse event
         */
        void mousePressEvent(QMouseEvent *e);
        /*!
         * \brief KeyReleaseEvent handler
         * \param e Key event
         */
        void keyReleaseEvent(QKeyEvent *e);
        /*!
         * \brief DoubleClickEvent handler
         * \param e Mouse event
         */
        void mouseDoubleClickEvent(QMouseEvent *e);
        /*!
         * \brief Addition of a skill
         * \param index Index where to add the skill
         */
        void addSkill(int index);
        /*!
         * \brief Addition of a character
         * \param index Index where to add the character
         */
        void addCharacter(int index);
        /*!
         * \brief Removal of a skill
         * \param index Index of the skill to be removed
         */
        void removeSkill(int index);
        /*!
         * \brief Removal of a character
         * \param index Index of the character to be removed
         */
        void removeCharacter(int index);
        /*!
         * \brief Edition of a skill
         * \param index Index of the skill to be edited
         */
        void editSkill(int index);
        /*!
         * \brief Edition of a character
         * \param index Index of the character to be edited
         */
        void editCharacter(int index);
    protected slots:
        /*!
         * \brief Slot for when a cell changes 
         * \param logicalRow Logical row of the cell
         * \param logicalColumn Logical column of the cell
         */
        void onCellChanged(int logicalRow, int logicalColumn);
        /*!
         * \brief Slot for when the horizontal header is right-clicked
         * \param index Index of the clicked column
         * \param position Global position of the click for popups
         */
        void onHHeaderClicked(int index, const QPoint &position);
        /*!
         * \brief Slot for when the vertical header is right-clicked
         * \param index Index of the clicked row
         * \param position Global position of the click for popups
         */
        void onVHeaderClicked(int index, const QPoint &position);
        /*!
         * \brief Slot for when a section of the horizontal header is moved
         * \param oldColumn Old visual column
         * \param newColumn New visual column
         */
        void onHHeaderMoved(int, int oldColumn, int newColumn);
        /*!
         * \brief Slot for when a section of the vertical header is moved
         * \param oldRow Old visual row
         * \param newRow New visual row
         */
        void onVHeaderMoved(int, int oldRow, int newRow);
        /*!
         * \brief Slot for when the selection changes 
         */
        void on_itemSelectionChanged();
    public:
        /*!
         * \brief Constructor of the widget
         * \param parent Parent widget
         */
        QCustomTableWidget(QWidget *parent=0);
        //! destructor
        ~QCustomTableWidget();
        /*!
         * \brief Setter for the underlying skill and character lists
         * \param skills Skill list
         * \param characters Character list
         *
         * Updates the display after the modification
         */
        void setLists(SkillList *skills, CharacterList *characters);
        /*!
         * \brief Convert visual to logical row index
         * \param visualRow Visual row
         * \return Logical row
         */
        int logicalRow(int visualRow);
        /*!
         * \brief Convert visual to logical column index
         * \param visualColumn Visual column
         * \return Logical column
         */
        int logicalColumn(int visualColumn);
    signals:
        /*!
         * \brief Signal to register a modification
         * \param modification Modification to register
         *
         * This signal is sent when the tree is modified
         */
        void modificationDone(Modification *modification);
    public slots:
        /*!
         * \brief Update of the display
         * \param row Row to focus on
         * \param column Column to focus on
         */
        void updateDisplay(int row = -1, int column = -1);
        /*!
         * \brief Scrolling
         * \param row Row index
         * \param column Column index
         *
         * Scroll to the item at the given indices. If one of the indices is equal to -1, no scrolling in this direction will be performed.
         */
        void scrollTo(int row, int column);
        /*!
         * \brief Update after a modification
         * \param modification Modification to undo or redo
         * \param undo True to undo, false to redo
         */
        void updateModification(CharacterModification *modification, bool undo);
};

#endif
