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

#ifndef HEADER_ITEMDIALOG
#define HEADER_ITEMDIALOG

#include "ui_ItemDialog.h"
#include "ItemFactory.h"

/*!
 * \brief Dialog window for item creation
 */
class ItemDialog: public QDialog, private Ui::itemDialog
{
    public:
        // result type
        enum Result {rBrother,rChild};
        // constructor
        ItemDialog(QWidget *parent=NULL);
        // accessors
        Item::State state() const;
        QString text() const;
        Item::Type type() const;
        /*!
         * \brief Getter for the name of the file
         *
         * If the file is within the current directory, returns the relative path
         */
        QString fileName() const;
        Result selectionResult() const;
    private:
        Result rRes;

    Q_OBJECT
    public slots:
        void on_pushChild_clicked();
        void on_pushBrother_clicked();
        /*!
         * \brief Slot for the "Basic" radio button
         *
         * Sets the type of the item to tBasic
         */
        void on_radioBasic_clicked();
        /*!
         * \brief Slot for the "Sound" radio item
         *
         * Sets the type of the item to tSound
         */
        void on_radioSound_clicked();
        /*!
         * \brief Slot for the "Browse" tool button
         *
         * Opens a dialog window to select a file
         */ 
        void on_toolBrowse_clicked();
        /*!
         * \brief Slot for the "Image" radio item
         *
         * Sets the type of the item to tImage
         */
        void on_radioImage_clicked();

        /*!
         * \brief Overriden "exec" slot
         * \param item Item to be edited for pre-filling
         * \return Result code
         *
         * Resets the interface and shows the window
         */
        int exec(Item *item = NULL);
};

#endif
