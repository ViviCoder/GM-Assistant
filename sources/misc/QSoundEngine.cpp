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

using namespace Phonon;

QSoundEngine::QSoundEngine(QWidget *parent): QObject(), musicObject(new MediaObject(parent)), soundObject(new MediaObject(parent)) 
{
    // music
    AudioOutput *musicOutput = new AudioOutput(MusicCategory, parent);
    Path musicPath = createPath(musicObject, musicOutput);
    musicObject->setTickInterval(TICK_INTERVAL);
    connect(musicObject, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(onMusicStateChanged(Phonon::State, Phonon::State))); 
    // sound
    AudioOutput *soundOutput = new AudioOutput(MusicCategory, parent);
    Path soundPath = createPath(soundObject, soundOutput);
    connect(soundObject, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(onSoundStateChanged(Phonon::State, Phonon::State))); 
}

bool QSoundEngine::isMusicPaused() const
{
    return musicObject->state() == PausedState;
}

void QSoundEngine::playSound(const QString &fileName)
{
    soundObject->setCurrentSource(MediaSource(fileName));
    soundObject->play();
}

void QSoundEngine::playMusic(const QString &fileName)
{
    musicObject->setCurrentSource(MediaSource(fileName));
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

void QSoundEngine::onMusicStateChanged(State newState, State)
{
    if (newState == ErrorState && musicObject->errorType() == FatalError)
    {
        emit errorOccured(musicObject->errorString());
    }
}

void QSoundEngine::onSoundStateChanged(State newState, State)
{
    if (newState == ErrorState)// && soundObject->errorType() == FatalError)
    {
        emit errorOccured(soundObject->errorString());
    }
}
