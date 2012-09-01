/*************************************************************************
* Copyright © 2012 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_NOTEMODIFICATION
#define HEADER_NOTEMODIFICATION

#include "Modification.h"
#include <string>

/*!
 * \brief Modification of the notes
 */
class NoteModification: public Modification
{
    public:
        /*!
         * \brief Constructor
         * \param note Modified note 
         * \param content Previous content
         * \param newContent Modified content
         */
        NoteModification(std::string &note, const std::string &content, const std::string &newContent);
        /*!
         * \brief Destructor
         */
        virtual ~NoteModification();
        // inherited pure virtual methods
        Type type() const;
        void undo();
        void redo();
    private:
        //! Modified note
        std::string &rNote;
        //! Previous content
        std::string sContent;
        //! Modified content
        std::string sNewContent;
};

#endif
