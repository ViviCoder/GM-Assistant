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

QSoundEngine::QSoundEngine(QWidget *parent): audioObject(new Phonon::MediaObject(parent)) 
{
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, parent);
    Phonon::Path path = Phonon::createPath(audioObject, audioOutput);

    audioObject->play();
}

bool QSoundEngine::isMusicPaused() const
{
    return audioObject->state() == Phonon::PausedState;
}

// methods

void QSoundEngine::playSound(const string &fileName) throw(runtime_error)
{
    Phonon::MediaObject *player = Phonon::createPlayer(Phonon::GameCategory, Phonon::MediaSource(fileName.c_str()));
    player->play();
    delete player;
}

void QSoundEngine::playMusic(const string &fileName) throw(runtime_error)
{
    audioObject->setCurrentSource(Phonon::MediaSource(fileName.c_str()));
    audioObject->play();
}

void QSoundEngine::pauseMusic()
{
    audioObject->pause();
}

void QSoundEngine::resumeMusic()
{
    audioObject->play();
}

bool QSoundEngine::isPlayingMusic() const
{
    return audioObject->state() == Phonon::PlayingState;
}

void QSoundEngine::move(double step)
{
    audioObject->seek((int)(step * 1000));
}

void QSoundEngine::stopMusic()
{
    audioObject->stop();
}

void QSoundEngine::stop()
{
    stopMusic();
}
