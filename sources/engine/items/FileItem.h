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

#ifndef HEADER_FILEITEM
#define HEADER_FILEITEM

#include "Item.h"

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
        //! Flag if included in archive or not
        bool bIncluded;
    public:
        /*!
         * \brief Setter for the name of the file of the item
         * \param fileName New file name
         * \param checkFile Indicates if the existence of the file is checked
         */
        void setFileName(const std::string &fileName, bool checkFile);
        /*!
         *  \brief Constructor
         *  \param content Content of the item
         *  \param state State of the item
         *  \param expanded Expanded/collapsed state of the item
         *  \param fileName Name of the file of the item
         */
        FileItem(const std::string &content="", State state=sNone, bool expanded=false, const std::string &fileName="");
        // accessors
        /*!
         * \brief Getter for the type of the item
         * \return tFile
         */
        Type type() const override;
        /*!
         * \brief Getter for the file name
         * \return The name of the file associated to the item
         */
        std::string fileName() const;
        // overriden XML-related methods
        /*!
         * \brief Loads the item from a XML tree
         * \param config IO configuration
         * \param root Position of the item in the XML tree
         * \param checkFile Indicates if the existence of the file is checked
         */
        void fromXML(const IOConfig &config, const Poco::XML::Element *root, bool checkFile) override;
        /*!
         * \brief Write the item to a XML tree
         * \param config IO configuration
         * \param root Position of the item in the XML tree
         * \param checkFile Indicates if the existence of the file is checked
         */
        void toXML(const IOConfig &config, Poco::XML::Element *root, FileMapping &fileMapping) override;
        /*!
         * \brief Getter for the archive subdirectory
         * \return Subdirectory of the archive in which the file is stored
         */
        virtual std::string subdirectory() const;
        /*!
         * \brief Getter for the inclusion flag
         * \return True if the item is included in an archive file, false otherwise
         */
        bool isIncluded() const;
};

inline Item::Type FileItem::type() const
{
    return tFile;
}

inline std::string FileItem::fileName() const
{
    return sFileName;
}

inline std::string FileItem::subdirectory() const
{
    return "misc/";
}

inline bool FileItem::isIncluded() const
{
    return bIncluded;
}

#endif
