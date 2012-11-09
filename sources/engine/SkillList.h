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

#ifndef HEADER_SKILLLIST
#define HEADER_SKILLLIST

#include <string>
#include <stdexcept>
#include <vector>
#include <libxml++/libxml++.h>

//! List of attributes/features/skills
class SkillList
{
    private:
        //! Underlying skill vector
        std::vector<std::string> vSkills;
    public:
        // iterator
        class iterator: public std::vector<std::string>::const_iterator
        {
            public:
                // constructor
                iterator(const std::vector<std::string>::const_iterator &it);
        };
        // constructor
        SkillList();
        // XML-related methods
        void toXML(xmlpp::Element &root) const;
        void fromXML(const xmlpp::Element &root);
        // accessor
        std::string& operator[](int index) throw(std::out_of_range);
        // populating
        void add(const std::string &skill, int position=-1);
        void remove(int index) throw(std::out_of_range);
        /*!
         * \brief Move of a skill
         * \param source Index of the skill to be moved
         * \param destination Index where to move the skill
         * \return True if the move has been made, false otherwise
         * \throw std::out_of_range Thrown when one of the indices is invalid
         */
        bool move(int source, int destination) throw (std::out_of_range);
        void clear();
        // iterators
        iterator begin() const;
        iterator end() const;
};

#endif
