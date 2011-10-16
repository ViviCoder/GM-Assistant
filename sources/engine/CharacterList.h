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

#ifndef HEADER_CHARACTERLIST
#define HEADER_CHARACTERLIST

#include "Character.h"
#include <stdexcept>
#include <vector>
#include <libxml++/libxml++.h>

class CharacterList
{
    private:
        std::vector<Character> vCharacters;
    public:
        // iterators
        class const_iterator: public std::vector<Character>::const_iterator
        {
            public:
                // constructor
                const_iterator(const std::vector<Character>::const_iterator &it);
        };
        class iterator: public std::vector<Character>::iterator
        {
            public:
                // constructor
                iterator(const std::vector<Character>::iterator &it);
                iterator(const const_iterator &it);
        };
        // constructor
        CharacterList();
        // XML-related methods
        void toXML(xmlpp::Element &root) const;
        void fromXML(const xmlpp::Element &root);
        // accessor
        Character& operator[](int index) throw(std::out_of_range);
        // populating
        void add(const std::string &character);
        void remove(int index) throw(std::out_of_range);
        void clear();
        // iterators
        const_iterator begin() const;
        const_iterator end() const;
        iterator begin();
        iterator end();
};

#endif
