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

#include "MetadataModification.h"

MetadataModification::MetadataModification(Metadata &metadata, const Metadata &newMetadata, const Metadata &oldMetadata): Modification(aEdition), mMetadata(metadata), mNewMetadata(newMetadata), mOldMetadata(oldMetadata)
{
}

Modification::Type MetadataModification::type() const
{
    return tMetadata;
}

void MetadataModification::undo()
{
    mMetadata = mOldMetadata;
}

void MetadataModification::redo()
{
    mMetadata = mNewMetadata;
}
