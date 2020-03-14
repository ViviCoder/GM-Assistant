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

#ifndef HEADER_METADATADIALOG
#define HEADER_METADATADIALOG

#include "ui_MetadataDialog.h"
#include "Metadata.h"

/*!
 * \brief Metadata editor
 */
class MetadataDialog: public QDialog, private Ui::metadataDialog
{
    private:
        //! Underlying metadata
        Metadata mMetadata;
    protected:
        /*!
         * \brief ChangeEvent handler
         * \param e Event to handle
         *
         * Updates the translation when changing language
         */
        void changeEvent(QEvent *e) override;
    public:
        /*!
         * \brief Constructor
         * \param parent Parent widget
         */
        MetadataDialog(QWidget *parent = 0);
        /*!
         * \brief Execution of the dialog window
         * \param metadata Current metadata
         * \return Result of the execution
         */
        int exec(const Metadata &metadata);
        /*!
         * \brief Slot for when the modifications have been accepted
         *
         * Modifies the underlying metadata
         */
        void accept() override;
        /*!
         * \brief Getter for the metadata
         * \return Metadata
         */
        Metadata metadata() const;
};

#endif
