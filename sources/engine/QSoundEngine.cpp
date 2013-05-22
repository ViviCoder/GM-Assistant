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

QSoundEngine::QSoundEngine(QWidget *parent): musicObject(new Phonon::MediaObject(parent)), soundObject(new Phonon::MediaObject(parent)) 
{
    // music
    Phonon::AudioOutput *musicOutput = new Phonon::AudioOutput(Phonon::MusicCategory, parent);
    Phonon::Path musicPath = Phonon::createPath(musicObject, musicOutput);
    // sound
    Phonon::AudioOutput *soundOutput = new Phonon::AudioOutput(Phonon::MusicCategory, parent);
    Phonon::Path soundPath = Phonon::createPath(soundObject, soundOutput);
}

bool QSoundEngine::isMusicPaused() const
{
    return musicObject->state() == Phonon::PausedState;
}

// methods

void QSoundEngine::playSound(const string &fileName) throw(runtime_error)
{
    soundObject->setCurrentSource(Phonon::MediaSource(fileName.c_str()));
    soundObject->play();
}

void QSoundEngine::playMusic(const string &fileName) throw(runtime_error)
{
    musicObject->setCurrentSource(Phonon::MediaSource(fileName.c_str()));
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
    return musicObject->state() == Phonon::PlayingState;
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
