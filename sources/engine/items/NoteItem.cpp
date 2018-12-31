/*************************************************************************
* Copyright © 2016-2018 Vincent Prat & Simon Nicolas
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

void NoteItem::fromXML(const IOConfig &config, const xmlpp::Element &root, bool) throw(xmlpp::exception)
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
}

void NoteItem::toXML(const IOConfig &config, xmlpp::Element &root, FileMapping&)
{
    using namespace xmlpp;

    if (config.hasNotes())
    {
        Element *tmp = root.add_child("note");
        pNote->toXML(*tmp);
    }
}
