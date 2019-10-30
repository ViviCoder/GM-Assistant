/*************************************************************************
* Copyright © 2012-2019 Vincent Prat & Simon Nicolas
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
#include "Note.h"

/*!
 * \brief Modification of the notes
 */
class NoteModification: public Modification
{
    public:
        //! Edition type
        enum EditionType
        {
            etTitle,
            etText
        };
        /*!
         * \brief Constructor for replacements
         * \param note Modified note
         * \param content Previous content
         * \param newContent Modified content
         * \param index Index of the modification
         */
        NoteModification(Note &note, const std::string &content, const std::string &newContent, int index);
        /*!
         * \brief Constructor for moves
         * \param note Modified note
         * \param index Index of the moved text
         * \param newIndex New index of the moved text
         * \param length Length of the moved text
         */
        NoteModification(Note &note, int index, int newIndex, int length);
        /*!
         * \brief Constructor for additions and deletions
         * \param note Modified note
         * \param action Action (addition or deletion)
         * \param content Added or deleted text
         * \param index Index of the modification
         */
        NoteModification(Note &note, Action action, const std::string &content, int index);
        /*!
         * \brief Constructor for renaming
         * \param note Modified note
         * \param title Previous title
         * \param newTitle Modified title
         */
        NoteModification(Note &note, const std::string &title, const std::string &newTitle);
        /*!
         * \brief Destructor
         */
        virtual ~NoteModification();
        // inherited pure virtual methods
        inline Type type() const;
        void undo();
        void redo();
        /*!
         * \brief Getter for the index
         * \return Index of the modification
         */
        inline int index() const;
        /*!
         * \brief Getter for the new index
         * \return New index of the modification
         */
        inline int newIndex() const;
        /*!
         * \brief Getter for the length
         * \return Length of the moved text
         */
        inline int length() const;
        /*!
         * \brief Getter for the modified content
         * \return Modified text
         */
        inline std::string content() const;
        /*!
         * \brief Getter for the new content
         * \return New text
         */
        inline std::string newContent() const;
        /*!
         * \brief Getter for the note
         *  \return Note
         */
        inline Note& note();
        /*!
         * \brief Getter for the edition type
         * \return Edition type
         */
        inline EditionType editionType() const;
    private:
        //! Modified note
        Note &rNote;
        //! Previous content
        std::string sContent;
        //! Modified content
        std::string sNewContent;
        //! Modification index
        int iIndex;
        //! Modification new index
        int iNewIndex;
        //! Length of the moved text
        int iLength;
        //! Edition type
        EditionType etEditType;
};

Modification::Type NoteModification::type() const
{
    return tNote;
}

int NoteModification::index() const
{
    return iIndex;
}

int NoteModification::newIndex() const
{
    return iNewIndex;
}

int NoteModification::length() const
{
    return iLength;
}

std::string NoteModification::content() const
{
    return sContent;
}

std::string NoteModification::newContent() const
{
    return sNewContent;
}

Note& NoteModification::note()
{
    return rNote;
}

NoteModification::EditionType NoteModification::editionType() const
{
    return etEditType;
}

#endif
