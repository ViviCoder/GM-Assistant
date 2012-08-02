/*************************************************************************
* Copyright © 2012 Vincent Prat & Simon Nicolas
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

#include "ModificationQueue.h"

using namespace std;

ModificationQueue::ModificationQueue(): vModifs(), iCurrent(vModifs.end())
{
}

ModificationQueue::~ModificationQueue()
{
    clear();
}

void ModificationQueue::clear()
{
    for (vector<Modification*>::iterator it = vModifs.begin(); it != vModifs.end(); it++)
    {
        delete *it;
    }
    vModifs.clear();
    iCurrent = vModifs.rend();
}

void ModificationQueue::add(Modification *modification)
{
    int i=0;
    // deletes undone modifications
    for (vector<Modification*>::reverse_iterator it = vModifs.rbegin(); it != iCurrent; it++)
    {
        delete *it;
        i++;
    }
    vModifs.resize(vModifs.size() - i);
    // adds the new modification
    vModifs.push_back(modification);
    iCurrent = vModifs.rbegin();
}

Modification* ModificationQueue::undo()
{
    if (iCurrent != vModifs.rend())
    {
        // there is a modification to undo
        Modification *modif = *iCurrent;
        modif->undo();
        iCurrent++;
        return modif;
    }
    return NULL;
}

Modification* ModificationQueue::redo()
{
    if (&(*(iCurrent - 1)) != &(*vModifs.end()))
    {
        // there is a modification to redo
        iCurrent--;
        (*iCurrent)->redo();
        return *iCurrent;
    }
    return NULL;
}

bool ModificationQueue::undoable() const
{
    return (iCurrent != vModifs.rend()); 
}

bool ModificationQueue::redoable() const
{
    return (&(*(iCurrent - 1)) != &(*vModifs.end())); 
}
