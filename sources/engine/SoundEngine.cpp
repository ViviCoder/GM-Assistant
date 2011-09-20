#include "SoundEngine.h"
#include <SDL_sound.h>

using namespace std;

// constructor
SoundEngine::SoundEngine() throw(runtime_error): iRate(44100), uFormat(MIX_DEFAULT_FORMAT), iChannels(MIX_DEFAULT_CHANNELS), iBufferSize(1024), mmMusic(NULL)
{
    SDL_Init(SDL_INIT_AUDIO);
    Sound_Init();

    if (Mix_OpenAudio(iRate,uFormat,iChannels,iBufferSize))
    {
        throw runtime_error("Unable to open the audio device");        
    }
    Mix_QuerySpec(&iRate,&uFormat,&iChannels);  // gathering real parameters
}

// destructor

SoundEngine::~SoundEngine()
{
    if (mmMusic != NULL)
    {
        Mix_FreeMusic(mmMusic);
    }
    Sound_Quit();
    Mix_CloseAudio();
    SDL_Quit();
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

// methods

void SoundEngine::playSound(const string &fileName) throw(runtime_error)
{
    Sound_Sample *sample = Sound_NewSampleFromFile(fileName.c_str(),NULL,iBufferSize);
    if (sample == NULL)
    {
        throw runtime_error("Unable to load the file");
    }
    int size = Sound_DecodeAll(sample);
    Mix_Chunk *mcSound = Mix_QuickLoad_RAW((Uint8 *)sample->buffer,size);
    Sound_FreeSample(sample);
    if (mcSound == NULL)
    {
        throw runtime_error("Unable to convert the file");
    }
    int channel = Mix_PlayChannel(-1,mcSound,0);
    if (channel < 0)
    {
        throw runtime_error("Unable to play the file");
    }
}

void SoundEngine::playMusic(const string &fileName) throw(runtime_error)
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
    while (!Mix_PlayingMusic())
    {
        Mix_PlayMusic(mmMusic,0);
    }
}
