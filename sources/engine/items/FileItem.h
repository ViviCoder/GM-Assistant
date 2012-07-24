/*************************************************************************
* Copyright © 2011-2012 Vincent Prat & Simon Nicolas
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
        bool bSizeLimited;
        /*!
         * \brief Limit size
         */
        int iLimitSize;
        /*!
         * \brief Setter for the name of the file of the item
         * \param fileName New file name
         * \param checkFile Indicates if the existence of the file is checked
         * \throw std::invalid_argument Thrown when the file is not readable
         * \throw std::overflow_error Thrown when the size of the file exceeds the limit (if limited)
         */
        virtual void setFileName(const std::string &fileName, bool checkFile) throw(std::invalid_argument, std::overflow_error);
    public:
        /*!
         *  \brief Constructor
         *  \param content Content of the item
         *  \param state State of the item
         *  \param expanded Expanded/collapsed state of the item
         *  \param fileName Name of the file of the item
         *  \param sizeLimited Indicates wether or not the size of the file is limited
         *  \param limitSize Limit size
         */
        FileItem(const std::string &content="", State state=sNone, bool expanded=false, const std::string &fileName="", bool sizeLimited = false, int limitSize = SIZE_LIMIT);
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
         * \param checkFile Indicates if the existence of the file is checked
         * \throw xmlpp::exception Thrown when there is an error in the XML tree
         * \throw std::invalid_argument Thrown when a file is not readable 
         * \throw std::overflow_error Thrown when the size of the file exceeds the limit (if limited)
         */
        void fromXML(const xmlpp::Element &root, bool checkFile) throw(xmlpp::exception, std::invalid_argument, std::overflow_error);
        void toXML(xmlpp::Element &root);
};

#endif
