/*************************************************************************
* Copyright © 2011 Free Software Fundation
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

#ifndef HEADER_SOUNDENGINE
#define HEADER_SOUNDENGINE

#include <string>
#include <stdexcept>
#include <SDL_mixer.h>
#include <SDL_sound.h>

class SoundEngine
{
	private:
        int iRate;
        Uint16 uFormat;
        int iChannels;  // 1 : mono / 2 : stereo
        int iBufferSize;
        // music
        Mix_Music *mmMusic;
        // sample of the sound
        Sound_Sample *ssSample;
	public:
		// constructor
		SoundEngine() throw(std::runtime_error);
        // destructor
        ~SoundEngine();
		// accessors
        int audioRate() const;
        Uint16 audioFormat() const;
        int audioChannels() const;
        int bufferSize() const;
        bool isPlayingMusic() const;
        // methods
		void playSound(const std::string &fileName) throw(std::runtime_error);
		void playMusic(const std::string &fileName) throw(std::runtime_error);
        void pauseMusic();
        void resumeMusic();
        // callback method
        static void onStopSound(int channel);
};

#endif