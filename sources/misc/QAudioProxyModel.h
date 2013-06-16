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

#ifndef HEADER_QAUDIOPROXYMODEL
#define HEADER_QAUDIOPROXYMODEL

#include <phonon>
#include <QSortFilterProxyModel>
#include <magic.h>

/*!
 * \brief Audio proxy model
 *
 * This proxy model keeps only audio files handled by Phonon.
 */
class QAudioProxyModel: public QSortFilterProxyModel
{
    public:
        /*!
         * \brief Default constructor
         * \param parent Parent widget
         */
        QAudioProxyModel(QWidget *parent = 0);
        /*!
         * \brief Destructor
         */
        ~QAudioProxyModel();
    protected:
        /*!
         * \brief Filter
         * \param sourceRow Row
         * \param sourceParent Index of the parent
         * \return True if the file is accepted, false otherwise
         *
         * Filters the files to keep only audio ones
         */
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    private:
        //! Magic cookie
        magic_t cookie;
        //! Supported formats
        QStringList slFormats;
};

#endif
