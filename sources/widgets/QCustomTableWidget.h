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
        SkillList *pSkills;
        CharacterList *pCharacters;
        /*!
         * \brief Flag indicating if an item is currently beeing edited
         */
        bool bEditing;

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

    protected slots:
        /*!
         * \brief Slot for when a cell changes 
         * \param row Row of the cell
         * \param column Column of the cell
         */
        void onCellChanged(int row, int column);
        void onHHeaderClicked(int index, const QPoint &position);
        void onVHeaderClicked(int index, const QPoint &position);
        void addSkill(int index);
        void addCharacter(int index);
        void removeSkill(int index);
        void removeCharacter(int index);
        void editSkill(int index);
        void editCharacter(int index);
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
        // destructor
        ~QCustomTableWidget();
        // associating a skill list and a character list to the widget
        void setLists(SkillList *skills, CharacterList *chars);
};

#endif
