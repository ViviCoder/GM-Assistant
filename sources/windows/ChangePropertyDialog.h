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

#ifndef HEADER_CHANGEPROPERTYDIALOG
#define HEADER_CHANGEPROPERTYDIALOG

#include "ui_ChangePropertyDialog.h"

/*!
 * \brief Dialog window for properties
 */
class ChangePropertyDialog: public QDialog, private Ui::changePropertyDialog
{
    Q_OBJECT

    public:
        /*!
         * \brief Constructor
         * \param parent Parent widget
         */
        ChangePropertyDialog(QWidget *parent=0);
        /*!
         * \brief Getter for the text
         * \return Text of the QLineEdit
         */
        QString text() const;
    public slots:
        /*!
         * \brief Slot for when the button is pressed
         */ 
        void on_pushEdit_clicked();
        /*!
         * \brief Overriden "exec" slot
         * \param text Pre-filled text
         * \return Result code
         *
         * Resets the interface and shows the window
         */
        int exec(QString text="");
    protected:
        /*!
         * \brief ChangeEvent handler
         * \param e Event to handle
         *
         * Updates the translation when changing language
         */
        void changeEvent(QEvent *e) override;
};

#endif
