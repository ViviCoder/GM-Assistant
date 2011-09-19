#ifndef HEADER_SOUNDENGINE
#define HEADER_SOUNDENGINE

#include <string>
#include <stdexcept>
#include <SDL.h>
#include <SDL_mixer.h>

class SoundEngine
{
	private:
        int iRate;
        Uint16 uFormat;
        int iChannels;  // 1 : mono / 2 : stereo
        int iBufferSize;
        // music
        Mix_Music *mmMusic;
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
