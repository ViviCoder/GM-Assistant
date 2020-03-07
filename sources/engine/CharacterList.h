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

#ifndef HEADER_CHARACTERLIST
#define HEADER_CHARACTERLIST

#include "Character.h"
#include <vector>
#include <Poco/DOM/Element.h>

//! List of characters
class CharacterList
{
    private:
        //! Underlying character vector
        std::vector<Character*> vCharacters;
    public:
        // iterators
        class const_iterator: public std::vector<Character*>::const_iterator
        {
            public:
                // constructor
                const_iterator(const std::vector<Character*>::const_iterator &it);
        };
        class iterator: public std::vector<Character*>::iterator
        {
            public:
                // constructor
                iterator(const std::vector<Character*>::iterator &it);
                iterator(const const_iterator &it);
        };
        // constructor
        CharacterList();
        //! Destructor
        ~CharacterList();
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
         * \brief Getter of the characters
         * \param index Index of the character
         * \return Pointer to the character at the given index
         * \throw std::out_of_range Thrown when the index does not correspond to any character
         */
        Character* operator[](int index);
        /*!
         * \brief Method to add a character
         * \param character Pointer to the character to add
         * \param position Position where to add the character
         *
         * If position is -1, the character is added at the end.
         */
        void add(Character *character, int position=-1);
        /*!
         * \brief Method to remove a character
         * \param index Index of the character to remove
         *
         * This method does not destroy the character.
         */
        void remove(int index);
        /*!
         * \brief Move of a character
         * \param source Index of the character to be moved
         * \param destination Index where to move the character
         * \return True if the move has been made, false otherwise
         */
        bool move(int source, int destination);
        /*!
         * \brief Method to clear the list
         *
         * Clear the lists and destroys all characters
         */
        void clear();
        // iterators
        const_iterator begin() const;
        const_iterator end() const;
        iterator begin();
        iterator end();
        /*!
         * \brief Getter for the number of characters
         * \return Number of characters
         */
        int count() const;
};

#endif
