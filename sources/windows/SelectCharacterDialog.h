/*************************************************************************
* Copyright © 2013 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_SELECTCHARACTERDIALOG
#define HEADER_SELECTCHARACTERDIALOG

#include "ui_SelectCharacterDialog.h"
#include "CharacterList.h"
#include "CombatDialog.h"

/*!
 * \brief Dialog window used to select the characters involved in a combat
 */
class SelectCharacterDialog: public QDialog, private Ui::selectCharacterDialog
{
    Q_OBJECT
    private:
        //! Combat manager dialog window
        CombatDialog *pCombat;
    protected:
        /*!
         * \brief ChangeEvent handler
         * \param e Event to handle
         *
         * Updates the translation when changing language
         */
        void changeEvent(QEvent *e);
    public:
        //! Default constructor
        SelectCharacterDialog(QWidget *parent);
    public slots:
        /*!
         * \brief Overriden slot for showing the dialog window
         * \param list List of characters
         *
         * Prepares the dialog window with the given list of characters
         */
        void show(const CharacterList &list);
        /*!
         * \brief Slot for the Add button
         *
         * Adds a character in the list of involved characters
         */
        void on_pushAdd_clicked();
        /*!
         * \brief Slot for the Remove button
         *
         * Removes a character from the list of involved characters
         */
        void on_pushRemove_clicked();
        /*!
         * \brief Slot for the Up button
         *
         * Put a character a slot higher
         */
        void on_pushUp_clicked();
        /*!
         * \brief Slot for the Down button
         *
         * Put a character a lot lower
         */
        void on_pushDown_clicked();
        // Overriden slot
        void accept();
};

#endif
