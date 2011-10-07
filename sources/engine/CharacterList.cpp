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

#include "CharacterList.h"

using namespace std;

CharacterList::CharacterList()
{
}

void CharacterList::toXML(xmlpp::Element &root) const
{
    using namespace xmlpp;

    for (vector<Character>::const_iterator it = vCharacters.begin(); it != vCharacters.end(); it++)
    {
        Element *tmp = root.add_child("character");
        tmp->set_attribute("name",it->name());
        tmp->set_attribute("playername",it->playerName());
        it->toXML(*tmp);
    }
}

void CharacterList::fromXML(const xmlpp::Element &root)
{
    using namespace xmlpp;

    clear();
    Node::NodeList node = root.get_children("character");
    for (Node::NodeList::const_iterator it = node.begin(); it != node.end(); it++)
    {
        Element *elem = dynamic_cast<Element*>(*it);
        string name;
        Attribute *attr = elem->get_attribute("name");
        if (attr != NULL)
        {
            name = attr->get_value();
        }
        string playerName="";
        attr = elem->get_attribute("playername");
        if (attr != NULL)
        {
            playerName = attr->get_value();
        }
        Character character = Character(name,playerName);
        character.fromXML(*elem);
        vCharacters.push_back(character);
    }        
}

void CharacterList::clear()
{
    vCharacters.clear();
}

void CharacterList::add(const std::string &character)
{
    vCharacters.push_back(character);
}

void CharacterList::remove(int index) throw(out_of_range)
{
    if (index < 0 || (unsigned int)index >= vCharacters.size())
    {
        throw out_of_range("Index out of bounds");
    }
    vCharacters.erase(vCharacters.begin()+index);
}

Character& CharacterList::operator[](int index) throw(out_of_range)
{
    if (index<0 || (unsigned int)index >= vCharacters.size())
    {
        throw out_of_range("Index out of bounds");
    }
    return vCharacters[index];
}

CharacterList::iterator CharacterList::begin() const
{
    return iterator(vCharacters.begin());
}

CharacterList::iterator CharacterList::end() const
{
    return iterator(vCharacters.end());
}

// iterator's method

CharacterList::iterator::iterator(const vector<Character>::const_iterator &it): vector<Character>::const_iterator(it)
{
}