#include "SoundEngine.h"

using namespace std;

// Constructor
SoundEngine::SoundEngine() throw(runtime_error)
{
    SDL_Init(SDL_INIT_AUDIO);
    Sound_Init();

    // set up
    int audio_rate = 44100;
    Uint16 audio_format = AUDIO_S16; 
    int audio_channels = 2;
    int audio_buffers = 1024;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
    {
        throw runtime_error("Unable to open the audio device");        
    }
}

// destructor

SoundEngine::~SoundEngine()
{
    Sound_Quit();
    Mix_CloseAudio();
    SDL_Quit();
}

// Methods

void SoundEngine::playSound(const std::string &fileName)
{
    Sound_Sample *sample = Sound_NewSampleFromFile(fileName.c_str(),NULL,1024);
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
