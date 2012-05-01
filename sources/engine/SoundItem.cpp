/*************************************************************************
* Copyright © 2011-2012 Vincent Prat & Simon Nicolas
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
#include "SoundEngine.h"

using namespace std;

SoundItem::SoundItem(const string &content, Item::State state, const string &fileName, bool sizeLimited): FileItem(content,state,fileName, sizeLimited, sizeLimited?SOUND_SIZE_LIMIT:SIZE_LIMIT), dDuration(0), bThreadFinished(true), pThread(NULL)
{
    if (fileName != "")
    {
        pThread = new QCustomThread(fileName,DEFAULT_BUFFER_SIZE,&dDuration,&bThreadFinished);
    }
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
        pThread = new QCustomThread(fileName,DEFAULT_BUFFER_SIZE,&dDuration,&bThreadFinished);
    }
}

void SoundItem::fromXML(const xmlpp::Element &root, bool checkFile) throw(xmlpp::exception, invalid_argument, overflow_error)
{
    FileItem::fromXML(root, checkFile);
    string name = fileName();
    if (name != "")
    {
        pThread = new QCustomThread(name,DEFAULT_BUFFER_SIZE,&dDuration,&bThreadFinished);
    }
}
