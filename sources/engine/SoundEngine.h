#ifndef HEADER_SOUNDENGINE
#define HEADER_SOUNDENGINE

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_sound.h>
#include <string>
#include <stdexcept>

class SoundEngine
{
	private:
	public:
		// constructor
		SoundEngine() throw(std::runtime_error);
        // destructor
        ~SoundEngine();
		// accessors
		void playSound(const std::string &fileName);
};

#endif
