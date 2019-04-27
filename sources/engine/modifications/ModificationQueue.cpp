/*************************************************************************
* Copyright © 2012-2018 Vincent Prat & Simon Nicolas
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

ModificationQueue::ModificationQueue(): vModifs(), iCurrent(0), iSaved(0)
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
    iCurrent = 0;
    iSaved = 0;
}

void ModificationQueue::add(Modification *modification)
{
    // deletes undone modifications
    for (int i = vModifs.size() - 1; i > iCurrent - 1; i--)
    {
        delete vModifs[i];
    }
    vModifs.resize(iCurrent);
    // adds the new modification
    vModifs.push_back(modification);
    iCurrent++;
}

Modification* ModificationQueue::undo()
{
    if (undoable())
    {
        // there is a modification to undo
        Modification *modif = vModifs[iCurrent-1];
        modif->undo();
        iCurrent--;
        return modif;
    }
    return 0;
}

Modification* ModificationQueue::redo()
{
    if (redoable())
    {
        // there is a modification to redo
        iCurrent++;
        Modification *modif = vModifs[iCurrent-1];
        modif->redo();
        return modif;
    }
    return 0;
}
