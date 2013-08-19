/*************************************************************************
* Copyright © 2013 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_IOCONFIG
#define HEADER_IOCONFIG

#include "Version.h"
#include <string>

/*!
 * \brief Input/Output configuration for games/scenarii
 *
 * Contains information about XML names among others
 */
class IOConfig
{
    private:
        //! Name of the XML root element 
        std::string sRootName;
        //! Name of the XML plot element
        std::string sPlotName;
        //! Name of the XML properties element
        std::string sPropertiesName;
        //! Name of the XML image element
        std::string sImageName;
        //! Name of the XML property element
        std::string sPropertyName;
        //! Flag of availability of image elements
        bool bHasImages;
        //! Flag of availability of the expanded state
        bool bHasExpanded;
        //! Version
        Version vVersion;
    public:
        /*!
         * \brief Constructor
         * \param version Software version to be compatible with
         */
        IOConfig(const Version &version);
        /*!
         * \brief Getter for the name of the root element
         * \return Name of the root element
         */
        std::string rootName() const;
        /*!
         * \brief Getter for the name of the plot element
         * \return Name of the plot element
         */
        std::string plotName() const;
        /*!
         * \brief Getter for the name of the properties element
         * \return Name of the properties element
         */
        std::string propertiesName() const;
        /*!
         * \brief Getter for the name of the image element
         * \return Name of the image element
         */
        std::string imageName() const;
        /*!
         * \brief Getter for the name of the property element
         * \return Name of the property element
         */
        std::string propertyName() const;
        /*!
         * \brief Getter for the flag of availability of image elements
         * \return Flag of availability of image elements
         */
        bool hasImages() const;
        /*!
         * \brief Getter for the flag of availability of the expanded state
         * \return Flag of availability of the expanded state
         */
        bool hasExpanded() const;
        /*!
         * \brief Getter for the version
         * \return Version of the file
         */
        Version version() const;
        /*!
         * \brief IOConfig detector
         * \param fileName Name of the file to scan
         * \return Pointer to an IO configuration corresponding to the given file
         */
        static IOConfig detect(const std::string &fileName);
};

#endif
