/*************************************************************************
* Copyright © 2013-2020 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_COMBATDIALOG
#define HEADER_COMBATDIALOG

#include "ui_CombatDialog.h"
#include <QCloseEvent>

/*!
 * \brief Dialog window which manages the action order in a turn-per-turn combat
 */
class CombatDialog: public QDialog, private Ui::combatDialog
{
    Q_OBJECT
    private:
        //! Counter
        int iCharacter;
        //! Vertical header
        QHeaderView *header;
    protected:
        /*!
         * \brief ChangeEvent handler
         * \param e Event to handle
         *
         * Updates the translation when changing language
         */
        void changeEvent(QEvent *e) override;
        /*!
         * \brief CloseEvent handler
         * \param e Event to handle
         *
         * Asks for confirmation before closing the dialog window
         */
        void closeEvent(QCloseEvent *e) override;
    public:
        //! Default constructor
        CombatDialog(QWidget *parent);
    public slots:
        /*!
         * \brief Overriden slot for showing the dialog window
         * \param list List of involved characters
         *
         * Prepares the dialog window with the given list of characters
         */
        void show(const QStringList &list);
        /*!
         * \brief Slot for when the Next button is clicked
         *
         * Goes to the next character to play
         */
        void on_pushNext_clicked();
        /*!
         * \brief Slot for when a character is moved
         * \param logicalIndex Logical index of the moved header section
         * \param oldVisualIndex Old visual index
         * \param newVisualIndex New visual index
         */
        void onCharacterMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
        /*!
         * \brief Slot for when the Remove button is clicked
         *
         * Deletes the current character
         */
        void on_pushRemove_clicked();
        /*!
         * \brief Slot to update the display
         */
        void updateDisplay();
        /*!
         * \brief Slot for when a character is double-clicked
         * \param logicalIndex Logical index of the header section
         */
        void onCharacterSelected(int logicalIndex);
        /*!
         * \brief Slot for when the Close button is clicked
         *
         * Closes the dialog window
         */
        void on_pushClose_clicked();
};

#endif
