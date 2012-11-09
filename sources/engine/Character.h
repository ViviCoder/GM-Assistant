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

#ifndef HEADER_CHARACTER
#define HEADER_CHARACTER

#include <vector>
#include <libxml++/libxml++.h>
#include <stdexcept>

//! Character (PC or NPC)
class Character
{
    private:
        std::string sName;
        std::string sPlayerName;
        //! Underlying vector
        std::vector<std::string> vSkills;
    public:
        // iterator
        class SkillIterator: public std::vector<std::string>::const_iterator
        {
            public:
                // constructor
                SkillIterator(const std::vector<std::string>::const_iterator &it);
        };
        // constructors
        Character(const std::string &name);    // for NPC
        Character(const std::string &name, const std::string &playerName);    // for PC
        // XML-related methods
        void toXML(xmlpp::Element &root) const;
        void fromXML(const xmlpp::Element &root);
        /*!
         * \brief Getter of the name
         * \return Character's name
         */
        std::string name() const;
        /*!
         * \brief Setter of the name
         * \param name New name
         */
        void setName(const std::string &name);
        /*!
         * \brief Getter of the player's name
         * \return Player's name
         */
        std::string playerName() const;
        /*!
         * \brief Setter of the player's name
         * \param playerName New player's name
         */
        void setPlayerName(const std::string &playerName);
        bool isPlaying() const;
        /*!
         * \brief Getter/setter of the skills
         * \param index Index of the skill
         * \return skill at the given index
         * \throw std::out_of_range Thrown when the given index does not correspond to any skill
         */
        std::string& skill(int index) throw(std::out_of_range);
        unsigned int skillNumber() const;
        // populating
        void addSkill(const std::string &skill, int position=-1);
        void removeSkill(int index) throw(std::out_of_range);
        /*!
         * \brief Move of a skill
         * \param source Index of the skill to be moved
         * \param destination Index where to move the skill
         * \return True if the move has been made, false otherwise
         * \throw std::out_of_range Thrown when one of the indices is invalid
         */
        bool moveSkill(int source, int destination) throw (std::out_of_range);
        void clearSkills();
        // iterators
        SkillIterator begin() const;
        SkillIterator end() const;
};

#endif
