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

#include "QCustomThread.h"
#include <SDL_sound.h>

using namespace std;

QCustomThread::QCustomThread(const string &fileName, int bufferSize, double *result, bool *finished): QThread(), sFileName(fileName), iBufferSize(bufferSize), dResult(result), bFinished(finished)
{
    // enables termination
    setTerminationEnabled(true);
    // in order to delete the thread after finishing
    connect(this,SIGNAL(finished()),this,SLOT(deleteLater()));
    // the thread is running
    start();
    *bFinished = false;
    *dResult = 0.0;
}

void QCustomThread::run()
{
    // decoding
    Sound_Sample *sample = Sound_NewSampleFromFile(sFileName.c_str(),NULL,iBufferSize);
    if (sample != NULL && sample->flags != SOUND_SAMPLEFLAG_NONE)
    {
        int totalSize=0,size;
        do
        {
            size = Sound_Decode(sample);
            totalSize += size;
            *dResult = 8192*(double)(totalSize) / (sample->actual.rate*sample->actual.format); 
        }
        while (size==iBufferSize && !*bFinished);
        Sound_FreeSample(sample);
    }
    *bFinished = true;
}
