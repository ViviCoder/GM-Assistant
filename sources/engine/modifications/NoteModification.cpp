/*************************************************************************
* Copyright © 2012-2013 Vincent Prat & Simon Nicolas
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

#include "NoteModification.h"

using namespace std;

NoteModification::NoteModification(string &note, const string &content, const string &newContent, int index): Modification(aEdition), rNote(note), sContent(content), sNewContent(newContent), iIndex(index)
{
}

NoteModification::NoteModification(string &note, int index, int newIndex, int length): Modification(aMovement), rNote(note), iIndex(index), iNewIndex(newIndex), iLength(length)
{
}

NoteModification::NoteModification(string &note, Action action, const string &content, int index): Modification(action), rNote(note), sContent(content), iIndex(index)
{
}

NoteModification::~NoteModification()
{
}

Modification::Type NoteModification::type() const
{
    return tNote;
}

void NoteModification::undo()
{
    int l = sContent.size();
    switch (action())
    {
        case aMovement: if (iIndex < iNewIndex)
                        {
                            rNote = rNote.substr(0, iIndex) + rNote.substr(iNewIndex - iLength, iLength) + rNote.substr(iIndex, iNewIndex - iIndex - iLength) + rNote.substr(iNewIndex);
                        }
                        else
                        {
                            rNote = rNote.substr(0, iNewIndex) + rNote.substr(iNewIndex + iLength, iIndex - iNewIndex) + rNote.substr(iNewIndex, iLength) + rNote.substr(iIndex + iLength);
                        }
                        break;
        case aEdition:  rNote = rNote.substr(0, iIndex) + sContent + rNote.substr(iIndex + sNewContent.size());
                        break;
        case aAddition: rNote = rNote.substr(0, iIndex) + rNote.substr(iIndex + l);
                        break;
        case aDeletion: rNote = rNote.substr(0, iIndex) + sContent + rNote.substr(iIndex);
                        break;
    }
}

void NoteModification::redo()
{
    int l = sContent.size();
    switch (action())
    {
        case aMovement: if (iIndex < iNewIndex)
                        {
                            rNote = rNote.substr(0, iIndex) + rNote.substr(iIndex + iLength, iNewIndex - iIndex - iLength) + rNote.substr(iIndex, iLength) + rNote.substr(iNewIndex);
                        }
                        else
                        {
                            rNote = rNote.substr(0, iNewIndex) + rNote.substr(iIndex, iLength) + rNote.substr(iNewIndex, iIndex - iNewIndex) + rNote.substr(iIndex + iLength);
                        }
                        break;
        case aEdition:  rNote = rNote.substr(0, iIndex) + sNewContent + rNote.substr(iIndex + l);
                        break;
        case aAddition: rNote = rNote.substr(0, iIndex) + sContent + rNote.substr(iIndex);
                        break;
        case aDeletion: rNote = rNote.substr(0, iIndex) + rNote.substr(iIndex + l);
                        break;
    }
}
