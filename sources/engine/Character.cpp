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

#include "Character.h"

using namespace std;

// constructors

Character::Character(const string &name): sName(name)
{
}

Character::Character(const string &name, const string &playerName): sName(name), sPlayerName(playerName)
{
}

// accessors

string Character::name() const
{
    return sName;
}

void Character::setName(const string &name)
{
    sName = name;
}

string Character::playerName() const
{
    return sPlayerName;
}

void Character::setPlayerName(const string &playerName)
{
    sPlayerName = playerName;
}

bool Character::isPlaying() const
{
    return sPlayerName == "";
}

std::string& Character::skill(int index) throw(out_of_range)
{
    if (index < 0 || (unsigned int)index >= vSkills.size())
    {
        throw out_of_range("Index out of bounds");
    }
    return vSkills[index];
}

unsigned int Character::skillNumber() const
{
    return vSkills.size();
}

// methods

void Character::toXML(xmlpp::Element &root) const
{
    using namespace xmlpp;

    for (vector<std::string>::const_iterator it = vSkills.begin(); it != vSkills.end(); it++)
    {
        Element *tmp = root.add_child("skill");
        tmp->set_attribute("value",*it);
    }
}

void Character::fromXML(const xmlpp::Element &root)
{
    using namespace xmlpp;

    clearSkills();
    Node::NodeList list = root.get_children("skill");
    for (Node::NodeList::const_iterator it = list.begin(); it != list.end(); it++)
    {
        Element *elem = dynamic_cast<Element *>(*it);
        string value;
        Attribute *attr = elem->get_attribute("value");
        if (attr)
        {
            value = attr->get_value();
        }
        vSkills.push_back(value);
    }
}

void Character::addSkill(const std::string &skill, int position)
{
    // if position is negative or greater than the size of the vector, just push_back
    if (position<0 || (unsigned int)position>vSkills.size())
    {
        vSkills.push_back(skill);
    }
    else
    {
        vSkills.insert(vSkills.begin()+position,skill);
    }
}

void Character::removeSkill(int index) throw(out_of_range)
{
    if (index < 0 || (unsigned int)index >= vSkills.size())
    {
        throw out_of_range("Index out of bounds");
    }
    vSkills.erase(vSkills.begin()+index);
}

void Character::clearSkills()
{
    vSkills.clear();
}

Character::SkillIterator Character::begin() const
{
    return SkillIterator(vSkills.begin());
}

Character::SkillIterator Character::end() const
{
    return SkillIterator(vSkills.end());
}

// iterator's method

Character::SkillIterator::SkillIterator(const vector<string>::const_iterator &it): vector<string>::const_iterator(it)
{
}
