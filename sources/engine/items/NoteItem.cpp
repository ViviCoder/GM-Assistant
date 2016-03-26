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

#include "NoteItem.h"

using namespace std;

NoteItem::NoteItem(const string &content, State state, bool expanded, const string &note): Item(content,state,expanded), sNote(note)
{
}

void NoteItem::fromXML(const IOConfig &config, const xmlpp::Element &root) throw(xmlpp::exception)
{
    if (config.hasNotes() && root.has_child_text())
    {
        sNote = root.get_child_text()->get_content();
    }
}

void NoteItem::toXML(const IOConfig &config, xmlpp::Element &root)
{
    using namespace xmlpp;

    if (config.hasNotes())
    {
        root.add_child_text(sNote);
    }
}
