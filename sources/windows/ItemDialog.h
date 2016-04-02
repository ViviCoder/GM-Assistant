/*************************************************************************
* Copyright © 2011-2016 Vincent Prat & Simon Nicolas
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
#include <QFileDialog>
#include "QAudioProxyModel.h"

/*!
 * \brief Dialog window for item creation
 */
class ItemDialog: public QDialog, private Ui::itemDialog
{
    Q_OBJECT
    public:
        // result type
        enum Result {rBrother,rChild};
        /*!
         * \brief Constructor
         * \param parent Parent widget
         * \param filter Filter for audio files
         */
        ItemDialog(QWidget *parent=0, QAudioProxyModel *filter=0);
        // accessors
        Item::State state() const;
        /*!
         * \brief Getter for the text of the item
         * \return Text of the item
         */
        inline QString text() const;
        /*!
         * \brief Getter for the type of the item
         * \return Type of the item
         */
        Item::Type type() const;
        /*!
         * \brief Getter for the name of the file
         *
         * If the file is within the current directory, returns the relative path
         */
        QString fileName() const;
        /*!
         * \brief Getter for the result of the selection
         * \return Result of the selection
         */
        inline Result selectionResult() const;
    public slots:
        /*!
         * \brief Slot for the "Child" push button
         *
         * Creates a new item as a child of the current item
         */
        void on_pushChild_clicked();
        /*!
         * \brief Slot for the "Brother" push button
         *
         * Creates a new item at the same level as the current item
         */
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
         * \brief Slot for the "Note" radio item
         *
         * Sets the type of the item to tNote
         */
        void on_radioNote_clicked();
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
        int exec(Item *item = 0);
    protected:
        /*!
         * \brief ChangeEvent handler
         * \param e Event to handle
         *
         * Updates the translation when changing language
         */
        void changeEvent(QEvent *e);
    private:
        //! Modal result
        Result rRes;
        //! Audio file browser
        QFileDialog *audioBrowser;
};

QString ItemDialog::text() const
{
    return editItem->text();
}

ItemDialog::Result ItemDialog::selectionResult() const
{
    return rRes;
}

#endif
