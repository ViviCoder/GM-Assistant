/*************************************************************************
* Copyright © 2011-2013 Vincent Prat & Simon Nicolas
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

#define SOUND_SIZE_LIMIT    1024

/*!
 * \brief Item related to a sound file 
 */
class SoundItem: public FileItem
{
    public:
        /*!
         * \brief Setter for the file name
         * \param fileName New file name
         * \param checkFile Indicates if the existence of the file is checked
         * \throw std::invalid_argument Thrown when the file is not readable
         * \throw std::overflow_error Thrown when the size of the file exceeds the limit (if limited)
         *
         * Calculates the duration of the sound file
         */
        void setFileName(const std::string &fileName, bool checkFile) throw(std::invalid_argument, std::overflow_error);
        /*!
         *  \brief Constructor
         *  \param content Content of the item
         *  \param state State of the item
         *  \param expanded Expanded/collapsed state of the item
         *  \param sizeLimited Indicates wether or not the size of the file is limited
         */
        SoundItem(const std::string &content="", State state=sNone, bool expanded=true, bool sizeLimited = false);
        /*!
         * \brief Getter for the type of the item
         * \return Item::tSound
         */
        Type type() const;
};

#endif
