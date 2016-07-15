/*************************************************************************
* Copyright © 2016 Vincent Prat & Simon Nicolas
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

#include "Note.h"
#include "Item.h"

using namespace std;

Note::Note(const string &title, const string &text, bool visible): sTitle(title), sText(text), bVisible(visible)
{
}

void Note::fromXML(const xmlpp::Element &root)
{
    using namespace xmlpp;

    Attribute *attr;
    // title
    attr = root.get_attribute("title");
    sTitle = "";
    if (attr)
    {
        sTitle = attr->get_value();
    }
    // visibility
    attr = root.get_attribute("visible");
    bVisible = false;
    if (attr)
    {
        bVisible = Item::strToBool(attr->get_value());
    }
    // text
    sText = "";
    if (root.has_child_text())
    {
        sText = root.get_child_text()->get_content();
    }
}

void Note::toXML(xmlpp::Element &root) const
{
    using namespace xmlpp;

    root.set_attribute("title", sTitle);
    root.set_attribute("visible", Item::boolToStr(bVisible));
    root.add_child_text(sText);
}
