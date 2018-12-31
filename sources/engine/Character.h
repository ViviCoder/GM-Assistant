/*************************************************************************
* Copyright © 2011-2018 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_CHARACTER
#define HEADER_CHARACTER

#include <vector>
#include <libxml++/libxml++.h>
#include <stdexcept>
#include "IOConfig.h"
#include "Note.h"

//! Character (PC or NPC)
class Character
{
    private:
        std::string sName;
        //! Short description of the character
        std::string sShort;
        //! Underlying vector
        std::vector<std::string> vProperties;
        //! Note associated with the character
        Note* pNote;
    public:
        // iterator
        class PropertyIterator: public std::vector<std::string>::const_iterator
        {
            public:
                // constructor
                PropertyIterator(const std::vector<std::string>::const_iterator &it);
        };
        /*!
         * \brief Constructor
         * \param name Character's name
         * \param shortDescription Short description (typically player's name for PCs or group/race for NPCs)
         */
        Character(const std::string &name, const std::string &shortDescription);
        /*!
         * \brief XML saver
         * \param config IO configuration
         * \param root Root of the XML tree
         */
        void toXML(const IOConfig &config, xmlpp::Element &root) const;
        /*!
         * \brief XML loader
         * \param config IO configuration
         * \param root Root of the XML tree
         */
        void fromXML(const IOConfig &config, const xmlpp::Element &root);
        /*!
         * \brief Getter of the name
         * \return Character's name
         */
        inline std::string name() const;
        /*!
         * \brief Setter of the name
         * \param name New name
         */
        inline void setName(const std::string &name);
        /*!
         * \brief Getter for the short description
         * \return Short description of the character
         */
        inline std::string shortDescription() const;
        /*!
         * \brief Setter for the short description
         * \param shortDescription New short description
         */
        inline void setShortDescription(const std::string &shortDescription);
        /*!
         * \brief Getter/setter of the properties
         * \param index Index of the property
         * \return property at the given index
         * \throw std::out_of_range Thrown when the given index does not correspond to any property
         */
        std::string& property(int index) throw(std::out_of_range);
        unsigned int propertyNumber() const;
        // populating
        void addProperty(const std::string &property, int position=-1);
        void removeProperty(int index) throw(std::out_of_range);
        /*!
         * \brief Move of a property
         * \param source Index of the property to be moved
         * \param destination Index where to move the property
         * \return True if the move has been made, false otherwise
         * \throw std::out_of_range Thrown when one of the indices is invalid
         */
        bool moveProperty(int source, int destination) throw (std::out_of_range);
        void clearProperties();
        /*!
         * \brief Getter for the note
         * \return Note associated with the character
         */
        inline Note *note();
        // iterators
        PropertyIterator begin() const;
        PropertyIterator end() const;
};

std::string Character::name() const
{
    return sName;
}

void Character::setName(const std::string &name)
{
    sName = name;
}

std::string Character::shortDescription() const
{
    return sShort;
}

void Character::setShortDescription(const std::string &shortDescription)
{
    sShort = shortDescription;
}

Note *Character::note()
{
    return pNote;
}

#endif
