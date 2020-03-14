/*************************************************************************
* Copyright © 2016-2020 Vincent Prat & Simon Nicolas
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
#include "Note.h"

/*!
 * \brief Item containing a note
 */
class NoteItem: public Item
{
    private:
        /*!
         * \brief Note associated with the item
         */
        Note *pNote;
    public:
        /*!
         * \brief Setter for the note
         * \param note New note
         */
        inline void setNote(Note *note);
        /*!
         * \brief Constructor
         * \param content Content of the item
         * \param state State of the item
         * \param note Note
         * \param expanded Expanded/collapsed state of the item
         *
         * If note is null, creates a new note
         */
        NoteItem(const std::string &content="", State state=sNone, bool expanded=false, Note *note=0);
        /*!
         * \brief Destructor
         */
        ~NoteItem();
        // accessors
        /*!
         * \brief Getter for the type of the item
         * \return tNote
         */
        inline Type type() const override;
        /*!
         * \brief Getter for the note
         * \return Note associated with the item
         */
        inline Note* note();
        // overriden XML-related methods
        /*!
         * \brief Loads the item from a XML tree
         * \param config IO configuration
         * \param root Position of the item in the XML tree
         * \param checkFiles Indicates if the existence of potential files is checked (useless here)
         * \throw xmlpp::exception Thrown when there is an error in the XML tree
         */
        void fromXML(const IOConfig &config, const Poco::XML::Element *root, bool checkFiles) override;
        /*!
         * \brief Saves the item into a XML tree
         * \param config IO configuration
         * \param root Position of the item in the XML tree
         * \param fileMapping Mapping of files associated with items (useless here)
         */
        void toXML(const IOConfig &config, Poco::XML::Element *root, FileMapping &fileMapping) override;
};

Item::Type NoteItem::type() const
{
    return tNote;
}

Note* NoteItem::note()
{
    return pNote;
}

void NoteItem::setNote(Note *note)
{
    pNote = note;
}

#endif
