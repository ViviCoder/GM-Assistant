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
        // methods
		void playSound(const std::string &fileName) throw(std::runtime_error);
		void playMusic(const std::string &fileName) throw(std::runtime_error);
};

#endif
