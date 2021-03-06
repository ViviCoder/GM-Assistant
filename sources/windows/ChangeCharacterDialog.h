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

#ifndef HEADER_CHANGECHARACTERDIALOG
#define HEADER_CHANGECHARACTERDIALOG

#include "ui_ChangeCharacterDialog.h"
#include "Character.h"

/*!
 * \brief Dialog window for characters
 */
class ChangeCharacterDialog: public QDialog, private Ui::changeCharacterDialog
{
    Q_OBJECT

    public:
        /*!
         * \brief Constructor
         * \param parent Parent widget
         */
        ChangeCharacterDialog(QWidget *parent=0);
        /*!
         * \brief Getter for the character's name
         * \return Character's name
         */
        inline QString name() const;
        /*!
         * \brief Getter for the short description of the character
         * \return Short description of the character
         */
        inline QString shortDescription() const;
    public slots:
        /*!
         * \brief Slot for when the button is pressed
         */ 
        void on_pushEdit_clicked();
        /*!
         * \brief Overriden "exec" slot
         * \param character Character (used for pre-filling)
         * \return Result code
         *
         * Resets the interface and shows the window
         */
        int exec(Character *character = 0);
    protected:
        /*!
         * \brief ChangeEvent handler
         * \param e Event to handle
         *
         * Updates the translation when changing language
         */
        void changeEvent(QEvent *e) override;
};

QString ChangeCharacterDialog::name() const
{
    return editName->text();
}

QString ChangeCharacterDialog::shortDescription() const
{
    return editShort->text();
}

#endif
