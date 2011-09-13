#include "soundengine.h"

using namespace std;

// Constructor
SoundEngine::SoundEngine():lenms(0), currentsound(0)
{
    sPath = "../data/sound/";
    /*
     * Create a system object and initialize.
    */
    result = FMOD::System_Create(&system);
    errcheck(result);

    result = system->setOutput(FMOD_OUTPUTTYPE_ALSA);
    errcheck(result);

    result = system->init(32, FMOD_INIT_NORMAL, 0);
    errcheck(result);
}

SoundEngine::SoundEngine(std::string &filename):lenms(0), currentsound(0)
{
    sFilename = filename;
    sPath = "../data/sound/";
    /*
     * Create a system object and initialize.
    */
    result = FMOD::System_Create(&system);
    errcheck(result);

    result = system->setOutput(FMOD_OUTPUTTYPE_ALSA);
    errcheck(result);

    result = system->init(32, FMOD_INIT_NORMAL, 0);
    errcheck(result);

//  result = system->createSound("cri.mp3", FMOD_SOFTWARE, &cri);
    errcheck(result);

    result = system->createSound(sFilename.c_str(), FMOD_SOFTWARE, 0,&sound);
    errcheck(result);

    sound->getLength(&lenms, FMOD_TIMEUNIT_MS);
    cout << "Duree totale: "<< lenms << " ms" << endl;

}

// Methods
//
void SoundEngine::errcheck(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) <<endl;
        exit(-1);
    }
}

void SoundEngine::playSound()
{
    result = system->playSound(FMOD_CHANNEL_FREE, sound, 0, &channel);
    errcheck(result);
    channel->getCurrentSound(&currentsound);
    result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);
    cout << "Duree totale: "<< lenms << " ms" << endl;

    Sleep(2000);
}
void SoundEngine::playSound(std::string &filename)
{
    sFilename = filename;

    result = system->createSound(sFilename.c_str(), FMOD_SOFTWARE, 0,&sound);
    errcheck(result);

    sound->getLength(&lenms, FMOD_TIMEUNIT_MS);
    cout << "Duree totale: "<< lenms << " ms" << endl;

    result = system->playSound(FMOD_CHANNEL_FREE, sound, 0, &channel);
    errcheck(result);
    Sleep(lenms);
    
    result = sound->release();
    errcheck(result);
}

SoundEngine::~SoundEngine()
{
    result = system->close();
    errcheck(result);
    result = system->release();
    errcheck(result);
}
