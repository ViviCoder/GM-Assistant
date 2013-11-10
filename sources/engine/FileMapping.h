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

#ifndef HEADER_FILEMAPPING
#define HEADER_FILEMAPPING

#include <set>
#include <map>
#include <string>

//! Class used to determine actual file names for archives
class FileMapping
{
    private:
        //! Set of added files
        std::set<std::string> sAddedFiles;
        //! Map of files
        std::map<std::string, std::string> mMapping;
    public:
        //! Constant iterator
        class Iterator
        {
            private:
                //! Underlying map iterator
                std::map<std::string, std::string>::const_iterator itMap;
            public:
                /*!
                 * \brief Constructor from a map iterator
                 * \param it Underlying map iterator
                 */
                Iterator(const std::map<std::string, std::string>::const_iterator &it);
                /*!
                 * \brief Getter for the current file name
                 * \return Current file name, "" if at the end of the iterator
                 */
                std::string file() const;
                /*!
                 * \brief Getter for the current destination
                 * \return Current destination, "" if at the end of the iterator
                 */
                std::string destination() const;
                /*!
                 * \brief Comparison operator
                 * \param it Iterator to compare with
                 * \return True if the iterators are differents, false otherwise
                 */
                bool operator!=(const Iterator &it) const;
                /*!
                 * \brief Incrementation operator
                 * \return Incremented iterator
                 *
                 * Increments the iterator
                 */
                Iterator operator++();
                /*!
                 * \brief Incrementation operator
                 * \param i Dummy parameter
                 * \return Non incremented iterator
                 *
                 * Increments the iterator 
                 */
                Iterator operator++(int i);
        };
        /*!
         * \brief File addition
         * \param fileName New file to map
         * \param destination Destination of the file in the archive
         * \return Actual file name of the file
         */
        std::string addFile(const std::string &fileName, const std::string &destination);
        /*!
         * \brief Beginning of the mapping
         * \return First position of the iterator
         */
        Iterator begin() const;
        /*!
         * \brief End of the mapping
         * \return Last position of the iterator
         */
        Iterator end() const;
};

inline FileMapping::Iterator FileMapping::begin() const
{
    return Iterator(mMapping.begin());
}

inline FileMapping::Iterator FileMapping::end() const
{
    return Iterator(mMapping.end());
}

// iterator methods

inline bool FileMapping::Iterator::operator!=(const FileMapping::Iterator &it) const
{
    return itMap != it.itMap;
}

inline std::string FileMapping::Iterator::file() const
{
    return itMap->first;
}

inline std::string FileMapping::Iterator::destination() const
{
    return itMap->second;
}

#endif
