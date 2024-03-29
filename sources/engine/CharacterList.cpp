/*************************************************************************
* Copyright © 2011-2021 Vincent Prat & Simon Nicolas
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
#include <Poco/DOM/NodeList.h>
#include <Poco/DOM/Document.h>
#include <stdexcept>

using namespace std;

CharacterList::CharacterList()
{
}

CharacterList::~CharacterList()
{
    clear();
}

void CharacterList::toXML(const IOConfig &config, Poco::XML::Element *root) const
{
    using namespace Poco::XML;

    Document *document = root->ownerDocument();
    for (vector<Character*>::const_iterator it = vCharacters.begin(); it != vCharacters.end(); it++)
    {
        Element *tmp = document->createElement("character");
        Character *character = *it;
        root->appendChild(tmp);
        tmp->setAttribute("name", character->name());
        tmp->setAttribute(config.descriptionName(), character->shortDescription());
        character->toXML(config, tmp);
    }
}

void CharacterList::fromXML(const IOConfig &config, const Poco::XML::Element *root)
{
    using namespace Poco::XML;

    clear();
    NodeList *list = root->getElementsByTagName("character");
    for (int i = 0; i < list->length(); i++)
    {
        Element *elem = static_cast<Element*>(list->item(i));
        string name = elem->getAttribute("name");
        string shortDescription = elem->getAttribute(config.descriptionName());
        Character *character = new Character(name, shortDescription);
        character->fromXML(config, elem);
        vCharacters.push_back(character);
    }
    list->release();
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

void CharacterList::remove(int index)
{
    if (index < 0 || (unsigned int)index >= vCharacters.size())
    {
        throw out_of_range("Index out of bounds");
    }
    vCharacters.erase(vCharacters.begin()+index);
}

bool CharacterList::move(int source, int destination)
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

Character* CharacterList::operator[](int index)
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
