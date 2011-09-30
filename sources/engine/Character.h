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

#ifndef HEADER_CHARACTER
#define HEADER_CHARACTER

#include <vector>
#include <libxml++/libxml++.h>
#include <stdexcept>

class Character
{
    private:
        std::string sName;
        std::string sPlayerName;
        //std::vector<Skill> vSkills;
        std::vector<std::string> vSkills;
    public:
        // constructors
        Character(const std::string &name);    // for NPC
        Character(const std::string &name, const std::string &playerName);    // for PC
        // XML-related methods
        void toXML(xmlpp::Element &root) const;
        void fromXML(const xmlpp::Element &root);
        // accessors
        std::string name() const;
        std::string playerName() const;
        bool isPlaying() const;
        std::string& skill(int index) throw(std::out_of_range);
        // populating
        void addSkill(const std::string &skill);
        void removeSkill(int index) throw(std::out_of_range);
        void clearSkills();
};

#endif
