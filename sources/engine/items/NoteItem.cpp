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

#include "NoteItem.h"
#include <Poco/DOM/Document.h>

using namespace std;

NoteItem::NoteItem(const string &content, State state, bool expanded, Note *note): Item(content,state,expanded), pNote(note)
{
    if (!note)
    {
        pNote = new Note();
    }
}

NoteItem::~NoteItem()
{
    if (pNote)
    {
        delete pNote;
    }
}

void NoteItem::fromXML(const IOConfig &config, const Poco::XML::Element *root, bool)
{
    using namespace Poco::XML;

    if (config.hasNotes())
    {
        Element *element  = root->getChildElement("note");
        if (element)
        {
            pNote->fromXML(element);
        }
    }
}

void NoteItem::toXML(const IOConfig &config, Poco::XML::Element *root, FileMapping&)
{
    using namespace Poco::XML;

    if (config.hasNotes())
    {
        Document *document = root->ownerDocument();
        Element *tmp = document->createElement("note");
        root->appendChild(tmp);
        pNote->toXML(tmp);
    }
}
