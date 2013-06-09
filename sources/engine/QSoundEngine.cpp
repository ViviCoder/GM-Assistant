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

#include "QSoundEngine.h"

using namespace std;
using namespace Phonon;

QSoundEngine::QSoundEngine(QWidget *parent): musicObject(new MediaObject(parent)), soundObject(new MediaObject(parent)) 
{
    // music
    AudioOutput *musicOutput = new AudioOutput(MusicCategory, parent);
    Path musicPath = createPath(musicObject, musicOutput);
    musicObject->setTickInterval(TICK_INTERVAL);
    // sound
    AudioOutput *soundOutput = new AudioOutput(MusicCategory, parent);
    Path soundPath = createPath(soundObject, soundOutput);
}

bool QSoundEngine::isMusicPaused() const
{
    return musicObject->state() == PausedState;
}

// methods

void QSoundEngine::playSound(const string &fileName) throw(runtime_error)
{
    soundObject->setCurrentSource(MediaSource(fileName.c_str()));
    soundObject->play();
}

void QSoundEngine::playMusic(const string &fileName) throw(runtime_error)
{
    musicObject->setCurrentSource(MediaSource(fileName.c_str()));
    musicObject->play();
}

void QSoundEngine::pauseMusic()
{
    musicObject->pause();
}

void QSoundEngine::resumeMusic()
{
    musicObject->play();
}

bool QSoundEngine::isPlayingMusic() const
{
    return musicObject->state() == PlayingState;
}

void QSoundEngine::move(double step)
{
    musicObject->seek((int)(step * 1000));
}

void QSoundEngine::stopMusic()
{
    musicObject->stop();
}

void QSoundEngine::stop()
{
    stopMusic();
    soundObject->stop();
}

MediaObject* QSoundEngine::musicPlayer() const
{
    return musicObject;
}

int QSoundEngine::musicDuration() const
{
    return (int)musicObject->totalTime();
}
