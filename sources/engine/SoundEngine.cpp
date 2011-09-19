#include "SoundEngine.h"
#include <SDL_sound.h>
#include <SDL_mixer.h>

using namespace std;

// constructor
SoundEngine::SoundEngine() throw(runtime_error): iRate(44100), uFormat(AUDIO_S16), iChannels(2), iBufferSize(1024), iMusicChannel(-1)
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

int SoundEngine::playFile(const string &fileName) throw(runtime_error)
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
    return channel;
}

void SoundEngine::playSound(const string &fileName) throw(runtime_error)
{
    int channel = playFile(fileName);
    if (channel==iMusicChannel)
    {
        // free the music channel if used by the sound (it means that music has finished)
        iMusicChannel = -1;
    }
}

void SoundEngine::playMusic(const string &fileName) throw(runtime_error)
{
    if (iMusicChannel != -1)
    {
        // stops music if the channel is busy
        Mix_HaltChannel(iMusicChannel);
        iMusicChannel = -1;
    }
    iMusicChannel = playFile(fileName);
}
