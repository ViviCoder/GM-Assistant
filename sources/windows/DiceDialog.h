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

#ifndef HEADER_DICEDIALOG
#define HEADER_DICEDIALOG

#include "ui_DiceDialog.h"
#include <vector>

/*!
 * \brief Dialog window for dice simulation
 */
class DiceDialog: public QDialog, private Ui::diceDialog
{
    Q_OBJECT
    public:
        /*! 
         * \brief Default constructor
         * \param parent Parent widget
         */
        DiceDialog(QWidget *parent);
    public slots:
        /*!
         * \brief Slot for when the Generate button is clicked
         *
         * Generates the requested random numbers
         */
        void on_pushThrow_clicked();
        /*!
         * \brief Slot for when the Reset button is clicked
         *
         * Resets the results
         */
        void on_pushReset_clicked();
        /*!
         * \brief Overriden slot for showing the dialog window
         *
         * Resets the results
         */
        void show();
        /*!
         * \brief Slot for when the number of dice equal to a certain number needs to be recomputed
         * \param value Value to compare to the dice
         */
        void on_spinEqual_valueChanged(int value);
        /*!
         * \brief Slot for when the number of dice greater or equal to a certain number needs to be recomputed
         * \param value Value to compare to the dice
         */
        void on_spinLeast_valueChanged(int value);
        /*!
         * \brief Slot for when the number of dice lower or equal to a certain number needs to be recomputed
         * \param value Value to compare to the dice
         */
        void on_spinMost_valueChanged(int value);
    protected:
        /*!
         * \brief ChangeEvent handler
         * \param e Event to handle
         *
         * Updates the translation when changing language
         */
        void changeEvent(QEvent *e) override;
    private:
        //! List of results
        std::vector<int> vResults;
};

#endif
