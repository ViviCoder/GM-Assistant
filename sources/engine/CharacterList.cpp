/*************************************************************************
* Copyright © 2011-2019 Vincent Prat & Simon Nicolas
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

CharacterList::~CharacterList()
{
    clear();
}

void CharacterList::toXML(const IOConfig &config, xmlpp::Element &root) const
{
    using namespace xmlpp;

    for (vector<Character*>::const_iterator it = vCharacters.begin(); it != vCharacters.end(); it++)
    {
        Element *tmp = root.add_child("character");
        Character *character = *it;
        tmp->set_attribute("name",character->name());
        tmp->set_attribute(config.descriptionName(), character->shortDescription());
        character->toXML(config, *tmp);
    }
}

void CharacterList::fromXML(const IOConfig &config, const xmlpp::Element &root)
{
    using namespace xmlpp;

    clear();
    Node::NodeList node = root.get_children("character");
    for (Node::NodeList::const_iterator it = node.begin(); it != node.end(); it++)
    {
        Element *elem = dynamic_cast<Element*>(*it);
        string name;
        Attribute *attr = elem->get_attribute("name");
        if (attr)
        {
            name = attr->get_value();
        }
        string shortDescription = "";
        attr = elem->get_attribute(config.descriptionName());
        if (attr)
        {
            shortDescription = attr->get_value();
        }
        Character *character = new Character(name, shortDescription);
        character->fromXML(config, *elem);
        vCharacters.push_back(character);
    }
}

void CharacterList::clear()
{
    for (vector<Character*>::iterator it = vCharacters.begin(); it != vCharacters.end(); it++)
    {
        delete (*it);
    }
    vCharacters.clear();
}

void CharacterList::add(Character *character, int position)
{
    // if out if bounds, just push_back
    if (position<0 || (unsigned int)position > vCharacters.size())
    {
        vCharacters.push_back(character);
    }
    else
    {
        vCharacters.insert(vCharacters.begin()+position,character);
    }
}

void CharacterList::remove(int index, bool toDelete) throw(out_of_range)
{
    if (index < 0 || (unsigned int)index >= vCharacters.size())
    {
        throw out_of_range("Index out of bounds");
    }
    vector<Character*>::iterator position = vCharacters.begin()+index;
    if (toDelete)
    {
        delete *position;
    }
    vCharacters.erase(position);
}

bool CharacterList::move(int source, int destination) throw(out_of_range)
{
    if (source < 0 || (unsigned int)source >= vCharacters.size())
    {
        throw out_of_range("Index of source out of bounds");
    }
    if (destination < 0 || (unsigned int)destination >= vCharacters.size())
    {
        throw out_of_range("Index of destination out of bounds");
    }
    if (source == destination)
    {
        return false;
    }
    // character to move
    Character *character = vCharacters[source];
    if (source < destination)
    {
        destination++;
    }
    else
    {
        source++;
    }
    vCharacters.insert(vCharacters.begin() + destination, character);
    vCharacters.erase(vCharacters.begin() + source);
    return true;
}

Character* CharacterList::operator[](int index) throw(out_of_range)
{
    if (index<0 || (unsigned int)index >= vCharacters.size())
    {
        throw out_of_range("Index out of bounds");
    }
    return vCharacters[index];
}

CharacterList::const_iterator CharacterList::begin() const
{
    return const_iterator(vCharacters.begin());
}

CharacterList::const_iterator CharacterList::end() const
{
    return const_iterator(vCharacters.end());
}

CharacterList::iterator CharacterList::begin()
{
    return iterator(vCharacters.begin());
}

CharacterList::iterator CharacterList::end()
{
    return iterator(vCharacters.end());
}

// iterators' methods

CharacterList::iterator::iterator(const vector<Character*>::iterator &it): vector<Character*>::iterator(it)
{
}

CharacterList::const_iterator::const_iterator(const vector<Character*>::const_iterator &it): vector<Character*>::const_iterator(it)
{
}

int CharacterList::count() const
{
    return vCharacters.size();
}
