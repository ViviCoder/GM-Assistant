/*************************************************************************
* Copyright © 2016 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_NOTEITEM
#define HEADER_NOTEITEM

#include "Item.h"

/*!
 * \brief Item containing a note
 */
class NoteItem: public Item
{
    private:
        /*!
         * \brief Content of the note associated with the item
         */
        std::string sNote;
    public:
        /*!
         * \brief Setter for the content of the note
         * \param note New content of the note
         */
        inline void setNote(const std::string &note);
        /*!
         *  \brief Constructor
         *  \param content Content of the item
         *  \param state State of the item
         *  \param expanded Expanded/collapsed state of the item
         *  \param note Content of the note
         */
        NoteItem(const std::string &content="", State state=sNone, bool expanded=false, const std::string &note="");
        // accessors
        /*!
         * \brief Getter for the type of the item
         * \return tNote
         */
        inline Type type() const;
        /*!
         * \brief Getter for the content of the note
         * \return The content of the note associated with the item
         */
        inline std::string note() const;
        // overriden XML-related methods
        /*!
         * \brief Loads the item from a XML tree
         * \param config IO configuration
         * \param root Position of the item in the XML tree
         * \throw xmlpp::exception Thrown when there is an error in the XML tree
         */
        void fromXML(const IOConfig &config, const xmlpp::Element &root) throw(xmlpp::exception);
        /*!
         * \brief Saves the item into a XML tree
         * \param config IO configuration
         * \param root Position of the item in the XML tree
         */
        void toXML(const IOConfig &config, xmlpp::Element &root);
};

Item::Type NoteItem::type() const
{
    return tNote;
}

std::string NoteItem::note() const
{
    return sNote;
}

void NoteItem::setNote(const std::string &note)
{
    sNote = note;
}

#endif
