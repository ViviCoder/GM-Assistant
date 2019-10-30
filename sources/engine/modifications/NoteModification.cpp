/*************************************************************************
* Copyright © 2012-2019 Vincent Prat & Simon Nicolas
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

NoteModification::NoteModification(Note &note, const string &content, const string &newContent, int index): Modification(aEdition), rNote(note), sContent(content), sNewContent(newContent), iIndex(index), etEditType(etText)
{
}

NoteModification::NoteModification(Note &note, int index, int newIndex, int length): Modification(aMovement), rNote(note), iIndex(index), iNewIndex(newIndex), iLength(length)
{
}

NoteModification::NoteModification(Note &note, Action action, const string &content, int index): Modification(action), rNote(note), sContent(content), iIndex(index)
{
}

NoteModification::NoteModification(Note &note, const string &title, const string &newTitle): Modification(aEdition), rNote(note), sContent(title), sNewContent(newTitle), etEditType(etTitle)
{
}

NoteModification::~NoteModification()
{
}

void NoteModification::undo()
{
    int l = sContent.size();
    string text = rNote.text();
    switch (action())
    {
        case aMovement: if (iIndex < iNewIndex)
                        {
                            rNote.text() = text.substr(0, iIndex) + text.substr(iNewIndex - iLength, iLength) + text.substr(iIndex, iNewIndex - iIndex - iLength) + text.substr(iNewIndex);
                        }
                        else
                        {
                            rNote.text() = text.substr(0, iNewIndex) + text.substr(iNewIndex + iLength, iIndex - iNewIndex) + text.substr(iNewIndex, iLength) + text.substr(iIndex + iLength);
                        }
                        break;
        case aEdition:  if (etEditType == etText)
                        {
                            rNote.text() = text.substr(0, iIndex) + sContent + text.substr(iIndex + sNewContent.size());
                        }
                        else
                        {
                            rNote.setTitle(sContent);
                        }
                        break;
        case aAddition: rNote.text() = text.substr(0, iIndex) + text.substr(iIndex + l);
                        break;
        case aDeletion: rNote.text() = text.substr(0, iIndex) + sContent + text.substr(iIndex);
                        break;
    }
}

void NoteModification::redo()
{
    int l = sContent.size();
    string text = rNote.text();
    switch (action())
    {
        case aMovement: if (iIndex < iNewIndex)
                        {
                            rNote.text() = text.substr(0, iIndex) + text.substr(iIndex + iLength, iNewIndex - iIndex - iLength) + text.substr(iIndex, iLength) + text.substr(iNewIndex);
                        }
                        else
                        {
                            rNote.text() = text.substr(0, iNewIndex) + text.substr(iIndex, iLength) + text.substr(iNewIndex, iIndex - iNewIndex) + text.substr(iIndex + iLength);
                        }
                        break;
        case aEdition:  if (etEditType == etText)
                        {
                            rNote.text() = text.substr(0, iIndex) + sNewContent + text.substr(iIndex + l);
                        }
                        else
                        {
                            rNote.setTitle(sNewContent);
                        }
                        break;
        case aAddition: rNote.text() = text.substr(0, iIndex) + sContent + text.substr(iIndex);
                        break;
        case aDeletion: rNote.text() = text.substr(0, iIndex) + text.substr(iIndex + l);
                        break;
    }
}
