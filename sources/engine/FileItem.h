/*************************************************************************
* Copyright © 2011 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_FILEITEM
#define HEADER_FILEITEM

#include "Item.h"

#define SIZE_LIMIT    20480

/*!
 * \brief Item related to a file
 */
class FileItem: public Item
{
    private:
        /*!
         * \brief Name of the file related to the item
         */
        std::string sFileName;
    protected:
        /*!
         * \brief Boolean indicating if the size of the sound file is limited
         */
        bool bLimitedSize;
        /*!
         * \brief Getter for the limit size
         * \return Limit size
         */
        virtual int limitSize() const;
        /*!
         * \brief Setter for the name of the file of the item
         * \param fileName New file name
         * \throw std::invalid_argument Thrown when the file is not readable
         * \throw std::overflow_error Thrown when the size of the file exceeds the limit (if limited)
         */
        virtual void setFileName(const std::string &fileName) throw(std::invalid_argument, std::overflow_error);
    public:
        /*!
         *  \brief Constructor
         *  \param content Content of the item
         *  \param state State of the item
         *  \param fileName Name of the file of the item
         *  \param limitedSize Indicates wether or not the size of the file is limited
         */
        FileItem(const std::string &content="", State state=sNone, const std::string &fileName="", bool limitedSize = false);
        // accessors
        /*!
         * \brief Getter for the type of the item
         * \return tFile
         */
        Type type() const;  // overriden
        std::string fileName() const;
        // overriden XML-related methods
        /*!
         * \brief Loads the item from a XML tree
         * \param root Position of the item in the XML tree
         * \throw xmlpp::exception Thrown when there is an error in the XML tree
         * \throw std::invalid_argument Thrown when a file is not readable 
         */
        void fromXML(const xmlpp::Element &root) throw(xmlpp::exception, std::invalid_argument);
        void toXML(xmlpp::Element &root);
};

#endif
