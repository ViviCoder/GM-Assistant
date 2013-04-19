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

#ifndef HEADER_COMBATDIALOG
#define HEADER_COMBATDIALOG

#include "ui_CombatDialog.h"

/*!
 * \brief Dialog window which manages the action order in a turn-per-turn combat
 */
class CombatDialog: public QDialog, private Ui::combatDialog
{
    Q_OBJECT
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
};

#endif