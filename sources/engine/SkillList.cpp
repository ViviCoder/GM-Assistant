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

#include "SkillList.h"

using namespace std;

SkillList::SkillList()
{
}

void SkillList::toXML(xmlpp::Element &root) const
{
    using namespace xmlpp;

    for (vector<string>::const_iterator it = vSkills.begin(); it != vSkills.end(); it++)
    {
        Element *tmp = root.add_child("skill");
        tmp->set_attribute("name",*it);
    }
}

void SkillList::fromXML(const xmlpp::Element &root)
{
    using namespace xmlpp;

    clear();
    Node::NodeList node = root.get_children("skill");
    for (Node::NodeList::const_iterator it = node.begin(); it != node.end(); it++)
    {
        Element *elem = dynamic_cast<Element*>(*it);
        string name;
        Attribute *attr = elem->get_attribute("name");
        if (attr)
        {
            name = attr->get_value();
        }
        vSkills.push_back(name);
    }
}

void SkillList::clear()
{
    vSkills.clear();
}

void SkillList::add(const std::string &skill, int position)
{
    // if position is equal to -1 or greater than the size of the list, just push_back
    if (position<0 || (unsigned int)position>vSkills.size())
    {
        vSkills.push_back(skill);
    }
    else
    {
        vSkills.insert(vSkills.begin()+position,skill);
    }
}

void SkillList::remove(int index) throw(out_of_range)
{
    if (index < 0 || (unsigned int)index >= vSkills.size())
    {
        throw out_of_range("Index out of bounds");
    }
    vSkills.erase(vSkills.begin()+index);
}

std::string& SkillList::operator[](int index) throw(out_of_range)
{
    if (index<0 || (unsigned int)index >= vSkills.size())
    {
        throw out_of_range("Index out of bounds");
    }
    return vSkills[index];
}

SkillList::iterator SkillList::begin() const
{
    return iterator(vSkills.begin());
}

SkillList::iterator SkillList::end() const
{
    return iterator(vSkills.end());
}

// iterator's method

SkillList::iterator::iterator(const vector<string>::const_iterator &it): vector<string>::const_iterator(it)
{
}
