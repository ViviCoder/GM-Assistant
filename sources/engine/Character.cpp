/*************************************************************************
* Copyright © 2011-2018 Vincent Prat & Simon Nicolas
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

Character::Character(const string &name, const string &shortDescription): sName(name), sShort(shortDescription), pNote(new Note())
{
}

std::string& Character::property(int index) throw(out_of_range)
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

void Character::toXML(const IOConfig &config, xmlpp::Element &root) const
{
    using namespace xmlpp;

    if (config.hasNotes())
    {
        Element *tmp = root.add_child("note");
        pNote->toXML(*tmp);
    }
    for (vector<std::string>::const_iterator it = vProperties.begin(); it != vProperties.end(); it++)
    {
        Element *tmp = root.add_child(config.propertyName());
        tmp->set_attribute("value",*it);
    }
}

void Character::fromXML(const IOConfig &config, const xmlpp::Element &root)
{
    using namespace xmlpp;

    if (config.hasNotes())
    {
        Node::NodeList node = root.get_children("note");
        if (!node.empty())
        {
            pNote->fromXML(*dynamic_cast<Element*>(node.front()));
        }
    }
    clearProperties();
    Node::NodeList list = root.get_children(config.propertyName());
    for (Node::NodeList::const_iterator it = list.begin(); it != list.end(); it++)
    {
        Element *elem = dynamic_cast<Element *>(*it);
        string value;
        Attribute *attr = elem->get_attribute("value");
        if (attr)
        {
            value = attr->get_value();
        }
        vProperties.push_back(value);
    }
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

void Character::removeProperty(int index) throw(out_of_range)
{
    if (index < 0 || (unsigned int)index >= vProperties.size())
    {
        throw out_of_range("Index out of bounds");
    }
    vProperties.erase(vProperties.begin()+index);
}

bool Character::moveProperty(int source, int destination) throw(out_of_range)
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
