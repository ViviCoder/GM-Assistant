/*************************************************************************
* Copyright © 2011-2013 Vincent Prat & Simon Nicolas
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

#include "PropertyList.h"

using namespace std;

PropertyList::PropertyList()
{
}

void PropertyList::toXML(const IOConfig &config, xmlpp::Element &root) const
{
    using namespace xmlpp;

    for (vector<string>::const_iterator it = vProperties.begin(); it != vProperties.end(); it++)
    {
        Element *tmp = root.add_child(config.propertyName());
        tmp->set_attribute("name",*it);
    }
}

void PropertyList::fromXML(const IOConfig &config, const xmlpp::Element &root)
{
    using namespace xmlpp;

    clear();
    Node::NodeList node = root.get_children(config.propertyName());
    for (Node::NodeList::const_iterator it = node.begin(); it != node.end(); it++)
    {
        Element *elem = dynamic_cast<Element*>(*it);
        string name;
        Attribute *attr = elem->get_attribute("name");
        if (attr)
        {
            name = attr->get_value();
        }
        vProperties.push_back(name);
    }
}

void PropertyList::clear()
{
    vProperties.clear();
}

void PropertyList::add(const std::string &property, int position)
{
    // if position is equal to -1 or greater than the size of the list, just push_back
    if (position<0 || (unsigned int)position>vProperties.size())
    {
        vProperties.push_back(property);
    }
    else
    {
        vProperties.insert(vProperties.begin()+position,property);
    }
}

void PropertyList::remove(int index) throw(out_of_range)
{
    if (index < 0 || (unsigned int)index >= vProperties.size())
    {
        throw out_of_range("Index out of bounds");
    }
    vProperties.erase(vProperties.begin()+index);
}

bool PropertyList::move(int source, int destination) throw(out_of_range)
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

std::string& PropertyList::operator[](int index) throw(out_of_range)
{
    if (index<0 || (unsigned int)index >= vProperties.size())
    {
        throw out_of_range("Index out of bounds");
    }
    return vProperties[index];
}

PropertyList::iterator PropertyList::begin() const
{
    return iterator(vProperties.begin());
}

PropertyList::iterator PropertyList::end() const
{
    return iterator(vProperties.end());
}

// iterator's method

PropertyList::iterator::iterator(const vector<string>::const_iterator &it): vector<string>::const_iterator(it)
{
}
