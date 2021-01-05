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

#include "Character.h"
#include <Poco/DOM/NodeList.h>
#include <Poco/DOM/Document.h>
#include <stdexcept>

using namespace std;

// constructors

Character::Character(const string &name, const string &shortDescription): sName(name), sShort(shortDescription)
{
}

std::string& Character::property(int index)
{
    if (index < 0 || (unsigned int)index >= vProperties.size())
    {
        throw out_of_range("Index out of bounds");
    }
    return vProperties[index];
}

unsigned int Character::propertyNumber() const
{
    return vProperties.size();
}

// methods

void Character::toXML(const IOConfig &config, Poco::XML::Element *root) const
{
    using namespace Poco::XML;

    Document *document = root->ownerDocument();
    for (vector<std::string>::const_iterator it = vProperties.begin(); it != vProperties.end(); it++)
    {
        Element *tmp = document->createElement(config.propertyName());
        root->appendChild(tmp);
        tmp->setAttribute("value", *it);
    }
}

void Character::fromXML(const IOConfig &config, const Poco::XML::Element *root)
{
    using namespace Poco::XML;

    clearProperties();
    NodeList *list = root->getElementsByTagName(config.propertyName());
    for (int i = 0; i < list->length(); i++)
    {
        Element *elem = static_cast<Element*>(list->item(i));
        string value = elem->getAttribute("value");
        vProperties.push_back(value);
    }
    list->release();
}

void Character::addProperty(const std::string &property, int position)
{
    // if position is negative or greater than the size of the vector, just push_back
    if (position<0 || (unsigned int)position>vProperties.size())
    {
        vProperties.push_back(property);
    }
    else
    {
        vProperties.insert(vProperties.begin()+position,property);
    }
}

void Character::removeProperty(int index)
{
    if (index < 0 || (unsigned int)index >= vProperties.size())
    {
        throw out_of_range("Index out of bounds");
    }
    vProperties.erase(vProperties.begin()+index);
}

bool Character::moveProperty(int source, int destination)
{
    if (source < 0 || (unsigned int)source >= vProperties.size())
    {
        throw out_of_range("Index of source out of bounds");
    }
    if (destination < 0 || (unsigned int)destination >= vProperties.size())
    {
        throw out_of_range("Index of destination out of bounds");
    }
    if (source == destination)
    {
        return false;
    }
    // property to move
    string property = vProperties[source];
    if (source < destination)
    {
        destination++;
    }
    else
    {
        source++;
    }
    vProperties.insert(vProperties.begin() + destination, property);
    vProperties.erase(vProperties.begin() + source);
    return true;
}

void Character::clearProperties()
{
    vProperties.clear();
}

Character::PropertyIterator Character::begin() const
{
    return PropertyIterator(vProperties.begin());
}

Character::PropertyIterator Character::end() const
{
    return PropertyIterator(vProperties.end());
}

// iterator's method

Character::PropertyIterator::PropertyIterator(const vector<string>::const_iterator &it): vector<string>::const_iterator(it)
{
}
