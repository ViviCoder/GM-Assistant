/*************************************************************************
* Copyright © 2011-2013 Vincent Prat & Simon Nicolas
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

#include "SoundItem.h"

using namespace std;

SoundItem::SoundItem(const string &content, Item::State state, bool expanded, bool sizeLimited): FileItem(content, state, expanded, "", sizeLimited, sizeLimited?SOUND_SIZE_LIMIT:SIZE_LIMIT), dDuration(0), bThreadFinished(true), pThread(0)
{
}

SoundItem::~SoundItem()
{
    // terminate the thread if still running
    bThreadFinished = true;
}

Item::Type SoundItem::type() const
{
    return tSound;
}

double SoundItem::duration() const
{
    return dDuration;
}

void SoundItem::setFileName(const string &fileName, bool checkFile) throw(invalid_argument, overflow_error)
{
    try
    {
        FileItem::setFileName(fileName, checkFile);
    }
    catch (overflow_error &e)
    {
        throw overflow_error(string(e.what()) + " Use music instead.");
    }

    // terminate the thread if still running
    dDuration = 0;
    bThreadFinished = true;
    if (fileName != "")
    {
        pThread = new QCustomThread(fileName, &dDuration, &bThreadFinished);
    }
}
