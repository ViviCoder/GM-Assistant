#ifndef HEADER_SOUNDENGINE
#define HEADER_SOUNDENGINE

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <stdexcept>

class SoundEngine
{
	private:
        Mix_Music *mmMusic;

	public:
		// constructor
		SoundEngine() throw(std::runtime_error);
        // destructor
        ~SoundEngine();
		// accessors
		void playSound(const std::string &fileName);
};

#endif
