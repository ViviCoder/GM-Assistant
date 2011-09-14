#ifndef HEADER_SOUNDENGINE
#define HEADER_SOUNDENGINE

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../libs/api/inc/fmod.hpp"
#include "../libs/api/inc/fmod_errors.h"
#include "../libs/common/wincompat.h"
#include <string>

class SoundEngine
{
	private:
		std::string sFilename, sPath;
        FMOD::System    *system;
        FMOD::Sound     *sound;
        FMOD::Channel   *channel;
        FMOD_RESULT     result;
        int             key;
        unsigned int    version;
        FMOD::Sound *currentsound;
        unsigned int    lenms;

	public:
		// constructor
		SoundEngine();
		SoundEngine(std::string &filename);
        // destructor
        ~SoundEngine();
		// accessors
		void playSound(const std::string &filename);
		void errcheck(FMOD_RESULT result);
        void clear();
};

#endif
