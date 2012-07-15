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

ModificationQueue::ModificationQueue(): dModifs(), iCurrent(dModifs.end())
{
}

ModificationQueue::~ModificationQueue()
{
    clear();
}

void ModificationQueue::clear()
{
    for (deque<Modification*>::iterator it = dModifs.begin(); it != dModifs.end(); it++)
    {
        delete *it;
    }
    dModifs.clear();
}

void ModificationQueue::add(Modification &newModification)
{
    // deletes undone modifications
    for (deque<Modification*>::iterator it = iCurrent+1; it != dModifs.end(); it++)
    {
        delete *it;
    }
    dModifs.erase(iCurrent+1, dModifs.end());
    // adds the new modification
    dModifs.push_back(&newModification);
    iCurrent++;
}
