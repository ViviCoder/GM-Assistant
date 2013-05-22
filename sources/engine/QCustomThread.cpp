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

#include "QCustomThread.h"

using namespace std;
using namespace Phonon;

QCustomThread::QCustomThread(const string &fileName, double *result, bool *finished): QThread(), sFileName(fileName), dResult(result), bFinished(finished), moPlayer(new MediaObject(this))   
{
    // enables termination
    setTerminationEnabled(true);
    // in order to delete the thread after finishing
    connect(this,SIGNAL(finished()),this,SLOT(deleteLater()));
    connect(moPlayer, SIGNAL(totalTimeChanged(qint64)), this, SLOT(finish(qint64)));
    // launches the thread
    start();
}

void QCustomThread::run()
{
    *bFinished = false;
    *dResult = 0.0;
    // determination of the duration
    moPlayer->setCurrentSource(MediaSource(sFileName.c_str()));
    moPlayer->play();
    exec();
}

void QCustomThread::finish(qint64 totalTime)
{
    if (!*bFinished)
    {
        moPlayer->stop();
        *dResult = totalTime / 1000.0;
        *bFinished = true;
        exit(0);
    }
}
