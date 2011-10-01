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

#ifndef HEADER_ENGINE
#define HEADER_ENGINE

#include "Tree.h"
#include "Character.h"

class Engine
{
    private:
        Tree tScenario,tHistory,tMusic,tEffects;
        std::string sNotes;
        std::vector<Character> vCharacters;
        std::vector<std::string> vSkills;
    public:
        // constructors
        Engine();
        Engine(const std::string &fileName) throw(xmlpp::exception);
        // loading and saving methods
        void fromFile(const std::string &fileName) throw(xmlpp::exception);
        void toFile(const std::string &fileName) const;
        // accessors
        Tree& scenario();
        std::string& notes();
        std::vector<Character> characterList();
        Character& character(int index) throw(std::out_of_range);
        Tree& history();
        Tree& music();
        Tree& effects();
        std::vector<std::string> skillList();
        std::string& skill(int index);
        void addSkill(const std::string &skill);
        void removeSkill(int idex) throw(std::out_of_range);
        // populating
        void addCharacter(const Character &character);
        void removeCharacter(int index) throw(std::out_of_range);
        void clear();
};

#endif
