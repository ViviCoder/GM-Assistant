/*************************************************************************
* Copyright © 2016-2020 Vincent Prat & Simon Nicolas
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
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Text.h>

using namespace std;

Note::Note(const string &title, const string &text, bool visible): sTitle(title), sText(text), bVisible(visible)
{
}

void Note::fromXML(const Poco::XML::Element *root)
{
    using namespace Poco::XML;

    sTitle = root->getAttribute("title");
    // visibility
    string attr = root->getAttribute("visible");
    bVisible = false;
    if (!attr.empty())
    {
        bVisible = Item::strToBool(attr);
    }
    // text
    sText = root->innerText();
}

void Note::toXML(Poco::XML::Element *root) const
{
    using namespace Poco::XML;

    root->setAttribute("title", sTitle);
    root->setAttribute("visible", Item::boolToStr(bVisible));
    Document *document = root->ownerDocument();
    root->appendChild(document->createTextNode(sText));
}
