#include "SoundEngine.h"
#include <stdexcept>

using namespace std;

// Constructor
SoundEngine::SoundEngine() throw(runtime_error): mmMusic(NULL)
{
    SDL_Init(SDL_INIT_AUDIO);

    // set up
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16; 
    int audio_channels = 2;
    int audio_buffers = 4096;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
    {
        throw runtime_error("Unable to open the audio device");        
    }
}

// destructor

SoundEngine::~SoundEngine()
{
    if (mmMusic != NULL)
    {
        Mix_FreeMusic(mmMusic);
    }
    Mix_CloseAudio();
    SDL_Quit();
}

// Methods

void SoundEngine::playSound(const std::string &fileName)
{
    if (mmMusic != NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(mmMusic);
        mmMusic = NULL;
    }
    mmMusic = Mix_LoadMUS(fileName.c_str());
    if (mmMusic == NULL)
    {
        throw runtime_error("Unable to load the file");
    }
    Mix_PlayMusic(mmMusic,0);
}
