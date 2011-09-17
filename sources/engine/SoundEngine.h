#ifndef HEADER_SOUNDENGINE
#define HEADER_SOUNDENGINE

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <stdexcept>

class SoundEngine
{
	private:
        Mix_Music *music;
        Mix_Chunk *sound;

	public:
		// constructor
		SoundEngine() throw(std::runtime_error);
        // destructor
        ~SoundEngine();
		// accessors
		void playSound(const std::string &filename);
//		void playMusic(const std::string &filename);
};

#endif
