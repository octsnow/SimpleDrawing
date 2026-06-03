#ifndef __SOUND_H__
#define __SOUND_H__

#include <string>
#include <windows.h>
#include <mmeapi.h>
#include <mmreg.h>

namespace Sound {
typedef struct sound_data_tag {
    HWAVEOUT wh;
    WAVEHDR whd;
} SOUNDDATA;

void Load(std::string filename, SOUNDDATA* pdata);
void SetVolume(SOUNDDATA* pdata, float volume);
void SetIsLooping(SOUNDDATA *pdata, int isLooping);
void Play(SOUNDDATA* pdata);
void Pause(SOUNDDATA *pdata);
void Stop(SOUNDDATA *pdata);
void DestroySoundData(SOUNDDATA* pdata);
}

#endif
