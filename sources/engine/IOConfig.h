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
        //! Name of the XML character's short description attribute
        std::string sDescriptionName; 
        //! Flag of availability of image elements
        bool bHasImages;
        //! Flag of availability of the expanded state
        bool bHasExpanded;
        //! Version
        Version vVersion;
        //! Flag of validity of the configuration
        bool bValid;
        //! Flag of availability of metadata
        bool bHasMetadata;
        //! Flag of archive format
        bool bArchived; 
        //! Temporary directory
        std::string sTempDir;
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
        inline std::string rootName() const;
        /*!
         * \brief Setter for the name of the root element
         * \param rootName New name of the root element
         */
        inline void setRootName(const std::string &rootName);
        /*!
         * \brief Getter for the name of the plot element
         * \return Name of the plot element
         */
        inline std::string plotName() const;
        /*!
         * \brief Setter for the name of the plot element
         * \param plotName New name of the plot element
         */
        inline void setPlotName(const std::string &plotName);
        /*!
         * \brief Getter for the name of the properties element
         * \return Name of the properties element
         */
        inline std::string propertiesName() const;
        /*!
         * \brief Setter for the name of the properties element
         * \param propertiesName New name of the properties element
         */
        inline void setPropertiesName(const std::string &propertiesName);
        /*!
         * \brief Getter for the name of the image element
         * \return Name of the image element
         */
        inline std::string imageName() const;
        /*!
         * \brief Setter for the name of the image element
         * \param imageName New name of the image element
         */
        inline void setImageName(const std::string &imageName);
        /*!
         * \brief Getter for the name of the property element
         * \return Name of the property element
         */
        inline std::string propertyName() const;
        /*!
         * \brief Setter for the name of the property element
         * \param propertyName New name of the property element
         */
        inline void setPropertyName(const std::string &propertyName);
        /*!
         * \brief Getter for the flag of availability of image elements
         * \return Flag of availability of image elements
         */
        inline bool hasImages() const;
        /*!
         * \brief Setter for the flag of availability of image elements
         * \param hasImages New flag of availability of image elements
         */
        inline void setHasImages(bool hasImages);
        /*!
         * \brief Getter for the flag of availability of the expanded state
         * \return Flag of availability of the expanded state
         */
        inline bool hasExpanded() const;
        /*!
         * \brief Setter for the flag of availability of the expanded state
         * \param hasExpanded New flag of availability of the expanded state
         */
        inline void setHasExpanded(bool hasExpanded);
        /*!
         * \brief Getter for the version
         * \return Version of the file
         */
        inline Version version() const;
        /*!
         * \brief IOConfig detector
         * \param fileName Name of the file to scan
         * \param isArchived Indicates if the game file is an archive
         * \return Pointer to an IO configuration corresponding to the given file
         */
        static IOConfig detect(const std::string &fileName, bool isArchived);
        /*!
         * \brief Getter for the flag of validity
         * \return Validity of the configuration
         */
        inline bool isValid() const;
        /*!
         * \brief Getter for the flag of availability of metadata
         * \return Flag of availability of metadata
         */
        inline bool hasMetadata() const;
        /*!
         * \brief Setter for the flag of availability of metadata
         * \param hadMetadata New flag of availability of metadata
         */
        inline void setHasMetadata(bool hadMetadata);
        /*!
         * \brief Getter for the flag of archive format
         * \return Flag of archive format
         */
        inline bool isArchived() const;
        /*!
         * \brief Setter for the flag of archive format
         * \param isArchived New flag of archive format
         */
        inline void setArchived(bool isArchived);
        /*!
         * \brief Getter for the temporary directory
         * \return Temporary directory
         */
        inline std::string temporaryDirectory() const;
        /*!
         * \brief Getter for the name of the short description attribute
         * \return Name of the short description attribute
         */
        inline std::string descriptionName() const;
        /*!
         * \brief Setter for the name of the short description attribute
         * \param descriptionName New name of the short description attribute
         */
        inline void setDescriptionName(const std::string &descriptionName);
};

std::string IOConfig::rootName() const
{
    return sRootName;
}

std::string IOConfig::plotName() const
{
    return sPlotName;
}

std::string IOConfig::propertiesName() const
{
    return sPropertiesName;
}

std::string IOConfig::imageName() const
{
    return sImageName;
}

Version IOConfig::version() const
{
    return vVersion;
}

std::string IOConfig::propertyName() const
{
    return sPropertyName;
}

bool IOConfig::hasImages() const
{
    return bHasImages;
}

bool IOConfig::hasExpanded() const
{
    return bHasExpanded;
}

bool IOConfig::isValid() const
{
    return bValid;
}

bool IOConfig::hasMetadata() const
{
    return bHasMetadata;
}

bool IOConfig::isArchived() const
{
    return bArchived;
}

void IOConfig::setRootName(const std::string &rootName)
{
    if (rootName != sRootName)
    {
        sRootName = rootName;
        bValid = false;
    }
}

void IOConfig::setPlotName(const std::string &plotName)
{
    if (plotName != sPlotName)
    {
        sPlotName = plotName;
        bValid = false;
    }
}

void IOConfig::setPropertiesName(const std::string &propertiesName)
{
    if (propertiesName != sPropertiesName)
    {
        sPropertiesName = propertiesName;
        bValid = false;
    }
}

void IOConfig::setPropertyName(const std::string &propertyName)
{
    if (propertyName != sPropertyName)
    {
        sPropertyName = propertyName;
        bValid = false;
    }
}

void IOConfig::setImageName(const std::string &imageName)
{
    if (imageName != sImageName)
    {
        sImageName = imageName;
        bValid = false;
    }
}

void IOConfig::setHasImages(bool hasImages)
{
    if (hasImages != bHasImages)
    {
        bHasImages = hasImages;
        bValid = false;
    }
}

void IOConfig::setHasExpanded(bool hasExpanded)
{
    if (hasExpanded != bHasExpanded)
    {
        bHasExpanded = hasExpanded;
        bValid = false;
    }
}

void IOConfig::setHasMetadata(bool hasMetadata)
{
    if (hasMetadata != bHasMetadata)
    {
        bHasMetadata = hasMetadata;
        bValid = false;
    }
}

void IOConfig::setArchived(bool isArchived)
{
    if (isArchived != bArchived)
    {
        bArchived = isArchived;
        bValid = false;
    }
}

std::string IOConfig::temporaryDirectory() const
{
    return sTempDir;
}

std::string IOConfig::descriptionName() const
{
    return sDescriptionName;
}

void IOConfig::setDescriptionName(const std::string &descriptionName)
{
    if (descriptionName != sDescriptionName)
    {
        sDescriptionName = descriptionName;
        bValid = false;
    }
}

#endif
