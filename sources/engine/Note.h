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

#ifndef HEADER_NOTE
#define HEADER_NOTE

#include <string>
#include <Poco/DOM/Element.h>

/*!
 * \brief Text note
 */
class Note
{
    public:
        /*!
         * \brief Default constructor
         * \param title Title of the note
         * \param text Text of the note
         * \param visible Flag that indicates if the note is visible
         */
        Note(const std::string &title = "", const std::string &text = "", bool visible = true);
        /*!
         * \brief Getter for the title
         * \return Title of the note
         */
        inline std::string title() const;
        /*!
         * \brief Setter for the title
         * \param title New title
         */
        inline void setTitle(const std::string &title);
        /*!
         * \brief Getter for the text
         * \return Reference to the text
         */
        inline std::string& text();
        /*!
         * \brief Const getter for the text
         * \return Text
         */
        inline const std::string& text() const;
        /*!
         * \brief Getter for the visibility
         * \return Visibility of the note
         */
        inline bool visible() const;
        /*!
         * \brief Setter for the visibility
         * \param visible New visibility
         */
        inline void setVisible(bool visible);
        /*!
         * \brief XML loader
         * \param root Root of the XML tree
         */
        void fromXML(const Poco::XML::Element *root);
        /*!
         * \brief XML saver
         * \param root Root of the XML tree
         */
        void toXML(Poco::XML::Element *root) const;
    private:
        //! Title
        std::string sTitle;
        //! Text
        std::string sText;
        //! Visibility
        bool bVisible;
};

// inline methods

std::string Note::title() const
{
    return sTitle;
}

void Note::setTitle(const std::string &title)
{
    sTitle = title;
}

bool Note::visible() const
{
    return bVisible;
}

void Note::setVisible(bool visible)
{
    bVisible = visible;
}

std::string& Note::text()
{
    return sText;
}

const std::string& Note::text() const
{
    return sText;
}

#endif
