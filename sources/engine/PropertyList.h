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

#ifndef HEADER_PROPERTYLIST
#define HEADER_PROPERTYLIST

#include <string>
#include <vector>
#include <Poco/DOM/Element.h>
#include "IOConfig.h"

//! List of attributes/features/properties
class PropertyList
{
    private:
        //! Underlying property vector
        std::vector<std::string> vProperties;
    public:
        // iterator
        class iterator: public std::vector<std::string>::const_iterator
        {
            public:
                // constructor
                iterator(const std::vector<std::string>::const_iterator &it);
        };
        // constructor
        PropertyList();
        /*!
         * \brief XML saver
         * \param config IO configuration
         * \param root Root of the XML subtree
         */
        void toXML(const IOConfig &config, Poco::XML::Element *root) const;
        /*!
         * \brief XML loader
         * \param config IO configuration
         * \param root Root of the XML subtree
         */
        void fromXML(const IOConfig &config, const Poco::XML::Element *root);
        /*!
         * \brief Getter
         * \param index Index of the property
         * \return Name of the property
         */
        std::string& operator[](int index);
        // populating
        void add(const std::string &property, int position=-1);
        /*!
         * \brief Remove a property
         * \param index Index of the property to remove
         */
        void remove(int index);
        /*!
         * \brief Move of a property
         * \param source Index of the property to be moved
         * \param destination Index where to move the property
         * \return True if the move has been made, false otherwise
         */
        bool move(int source, int destination);
        void clear();
        // iterators
        iterator begin() const;
        iterator end() const;
};

#endif
