/*************************************************************************
* Copyright © 2011 Vincent Prat & Simon Nicolas
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

#include "SoundEngine.h"

using namespace std;

// constructor
SoundEngine::SoundEngine() throw(runtime_error): iRate(44100), uFormat(MIX_DEFAULT_FORMAT), iChannels(MIX_DEFAULT_CHANNELS), iBufferSize(1024), mmMusic(NULL), ssSample(NULL), dDuration(0.0), pThread(NULL), bThreadFinished(true) 
{
    Sound_Init();

    if (Mix_OpenAudio(iRate,uFormat,iChannels,iBufferSize))
    {
        throw runtime_error("Unable to open the audio device");        
    }
    Mix_QuerySpec(&iRate,&uFormat,&iChannels);  // gathering real parameters
    // allocating the sound channel
    Mix_AllocateChannels(1);
}

// destructor

SoundEngine::~SoundEngine()
{
    if (mmMusic != NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(mmMusic);
    }
    if (ssSample != NULL)
    {
        Mix_HaltChannel(0);
        Sound_FreeSample(ssSample);
    }
    // terminating the thread
    bThreadFinished = true;

    Sound_Quit();
    Mix_CloseAudio();
}

// accessors

int SoundEngine::audioRate() const
{
    return iRate;
}

Uint16 SoundEngine::audioFormat() const
{
    return uFormat;
}

int SoundEngine::audioChannels() const
{
    return iChannels;
}

int SoundEngine::bufferSize() const
{
    return iBufferSize;
}

double SoundEngine::duration()
{
    return dDuration;
}

bool SoundEngine::isMusicPaused() const
{
    return Mix_PausedMusic();
}

// methods

void SoundEngine::playSound(const string &fileName) throw(runtime_error)
{
    // change the volume of the music
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    if (ssSample != NULL)
    {
        Mix_HaltChannel(0);
        Sound_FreeSample(ssSample);
        ssSample = NULL;
    }
    ssSample = Sound_NewSampleFromFile(fileName.c_str(),NULL,iBufferSize);
    if (ssSample == NULL)
    {
        throw runtime_error("Unable to load the file");
    }
    int size = Sound_DecodeAll(ssSample);
    Mix_Chunk *mcSound = Mix_QuickLoad_RAW((Uint8 *)ssSample->buffer,size);
    if (mcSound == NULL)
    {
        throw runtime_error("Unable to convert the file");
    }
    int channel = Mix_PlayChannel(0,mcSound,0);
    if (channel < 0)
    {
        throw runtime_error("Unable to play the file");
    }
    // callback when finished
    Mix_ChannelFinished(onStopSound);
}

void SoundEngine::playMusic(const string &fileName) throw(runtime_error)
{
    if (mmMusic != NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(mmMusic);
        mmMusic = NULL;
    }
    // terminating the thread if still running
    bThreadFinished = true;
    mmMusic = Mix_LoadMUS(fileName.c_str());
    if (mmMusic == NULL)
    {
        throw runtime_error("Unable to load the file");
    }
    while (!Mix_PlayingMusic())
    {
        Mix_PlayMusic(mmMusic,0);
    }
    pThread = new QCustomThread(fileName,iBufferSize,&dDuration,&bThreadFinished); 
}

void SoundEngine::onStopSound(int channel)
{
    Mix_VolumeMusic(MIX_MAX_VOLUME);
}

void SoundEngine::pauseMusic()
{
    Mix_PauseMusic();
}

void SoundEngine::resumeMusic()
{
    Mix_ResumeMusic();
}

bool SoundEngine::isPlayingMusic() const
{
    return Mix_PlayingMusic();
}

void SoundEngine::move(double step)
{
    if (mmMusic != NULL)
    {
        Mix_SetMusicPosition(step);
    }
}

void SoundEngine::goTo(double position)
{
    if (mmMusic != NULL)
    {
        Mix_RewindMusic();
        Mix_SetMusicPosition(position);
    }
}
