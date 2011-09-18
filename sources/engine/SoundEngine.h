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
        int iRate;
        Uint16 uFormat;
        int iChannels;  // 1 : mono / 2 : stereo
        int iBufferSize;
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
};

#endif
