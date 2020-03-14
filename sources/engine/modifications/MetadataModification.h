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

#ifndef HEADER_METADATAMODIFICATION
#define HEADER_METADATAMODIFICATION

#include "Modification.h"
#include "Metadata.h"

/*!
 * \brief Modification of the metadata
 */
class MetadataModification: public Modification
{
    public:
        /*!
         * \brief Constructor
         * \param metadata Modified metadata
         * \param newMetadata New metadata
         * \param oldMetadata Old metadata
         */
        MetadataModification(Metadata &metadata, const Metadata &newMetadata, const Metadata &oldMetadata);
        // inherited pure virtual methods
        /*!
         * \brief Getter for the type
         * \return tMetadata
         */
        Type type() const override;
        //! Undo the modification
        void undo() override;
        //! Redo the modification
        void redo() override;
    private:
        //! Modified metadata
        Metadata &mMetadata;
        //! New metadata
        Metadata mNewMetadata;
        //! Old metadata
        Metadata mOldMetadata;
};

#endif
