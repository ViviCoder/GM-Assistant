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

#ifndef HEADER_SOUNDITEM
#define HEADER_SOUNDITEM

#include "FileItem.h"

/*!
 * \brief Item related to a sound file 
 */
class SoundItem: public FileItem
{
    public:
        /*!
         *  \brief Constructor
         *  \param content Content of the item
         *  \param state State of the item
         *  \param expanded Expanded/collapsed state of the item
         */
        SoundItem(const std::string &content="", State state=sNone, bool expanded=true);
        /*!
         * \brief Getter for the type of the item
         * \return Item::tSound
         */
        Type type() const override;
        /*!
         * \brief Getter for the archive subdirectory corresponding to the item type
         * \return Sound subdirectory
         */
        std::string subdirectory() const override;
};

inline Item::Type SoundItem::type() const
{
    return tSound;
}

inline std::string SoundItem::subdirectory() const
{
    return "sounds/";
}

#endif
