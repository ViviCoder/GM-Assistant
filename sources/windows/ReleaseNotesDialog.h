/*************************************************************************
* Copyright © 2020 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_RELEASENOTESDIALOG
#define HEADER_RELEASENOTESDIALOG

#include "ui_ReleaseNotesDialog.h"

/*!
 * \brief Dialog window for release notes
 */
class ReleaseNotesDialog: public QDialog, private Ui::releaseNotesDialog
{
    Q_OBJECT
    public:
        /*! 
         * \brief Default constructor
         * \param parent Parent widget
         * \param instal_dir Installation directory
         */
        ReleaseNotesDialog(QWidget *parent, const QString &install_dir);
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
