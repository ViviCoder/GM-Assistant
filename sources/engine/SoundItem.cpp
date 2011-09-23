/*************************************************************************
* Copyright © 2011 Free Software Fundation
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

#include "SoundItem.h"

using namespace std;

SoundItem::SoundItem(const string &content, State state): Item(content,state)
{
}

Item::Type SoundItem::type() const
{
    return tSound;
}

string SoundItem::fileName() const
{
    return sFileName;
}

void SoundItem::setFileName(const string &fileName)
{
    sFileName = fileName;
}

void SoundItem::fromXML(const xmlpp::Element &root) throw(xmlpp::exception)
{
    using namespace xmlpp;
    
    Node::NodeList list = root.get_children("file");
    if (list.size()==0)
    {
        throw xmlpp::exception("Missing file name");
    }
    else
    {
        Element *tmp = dynamic_cast<Element*>(list.front());
        sFileName = "";
        Attribute *attr = tmp->get_attribute("name");
        if (attr!=NULL)
        {
            sFileName = attr->get_value();
        }
    }
}

void SoundItem::toXML(xmlpp::Element &root)
{
    using namespace xmlpp;

    Element *tmp = root.add_child("file");
    tmp->set_attribute("name",sFileName);
}
