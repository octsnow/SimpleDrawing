#include "sound.hpp"
#include "Wav.hpp"

void Sound::Load(std::string filename, Sound::SOUNDDATA* pdata) {
    char *data;
    wav::WAVEINFO wi;
    WAVEFORMATEX wf;

    wav::read(filename, (unsigned char**)&data, &wi);

    if(wi.format_type == wav::WaveFormatType::WAVEFORMAT) {
        wf.wFormatTag = wi.info.waveformat.w_format_tag;
        wf.nChannels = wi.info.waveformat.n_channels;
        wf.nSamplesPerSec = wi.info.waveformat.n_samples_per_sec;
        wf.nAvgBytesPerSec = wi.info.waveformat.n_avg_bytes_per_sec;
        wf.nBlockAlign = wi.info.waveformat.n_block_align;
        wf.wBitsPerSample = 8;
        wf.cbSize = 0;
    } else if(wi.format_type == wav::WaveFormatType::PCMWAVEFORMAT) {
        wf.wFormatTag = wi.info.pcm_waveformat.wf.w_format_tag;
        wf.nChannels = wi.info.pcm_waveformat.wf.n_channels;
        wf.nSamplesPerSec = wi.info.pcm_waveformat.wf.n_samples_per_sec;
        wf.nAvgBytesPerSec = wi.info.pcm_waveformat.wf.n_avg_bytes_per_sec;
        wf.nBlockAlign = wi.info.pcm_waveformat.wf.n_block_align;
        wf.wBitsPerSample = wi.info.pcm_waveformat.w_bits_per_sample;
        wf.cbSize = 0;
    } else if(wi.format_type == wav::WaveFormatType::WAVEFORMATEX) {
        wf.wFormatTag = wi.info.waveformat_ex.w_format_tag;
        wf.nChannels = wi.info.waveformat_ex.n_channels;
        wf.nSamplesPerSec = wi.info.waveformat_ex.n_samples_per_sec;
        wf.nAvgBytesPerSec = wi.info.waveformat_ex.n_avg_bytes_per_sec;
        wf.nBlockAlign = wi.info.waveformat_ex.n_block_align;
        wf.wBitsPerSample = wi.info.waveformat_ex.w_bits_per_sample;
        wf.cbSize = wi.info.waveformat_ex.cb_size;
    } else if(wi.format_type == wav::WaveFormatType::WAVEFORMATEXTENSIBLE) {
        wf.wFormatTag = wi.info.waveformat_extensible.format.w_format_tag;
        wf.nChannels = wi.info.waveformat_extensible.format.n_channels;
        wf.nSamplesPerSec = wi.info.waveformat_extensible.format.n_samples_per_sec;
        wf.nAvgBytesPerSec = wi.info.waveformat_extensible.format.n_avg_bytes_per_sec;
        wf.nBlockAlign = wi.info.waveformat_extensible.format.n_block_align;
        wf.wBitsPerSample = wi.info.waveformat_extensible.format.w_bits_per_sample;
        wf.cbSize = wi.info.waveformat_extensible.format.cb_size;
    }

    MMRESULT res = waveOutOpen(&(pdata->wh), WAVE_MAPPER, &wf, NULL, NULL, CALLBACK_NULL);

    if(res != MMSYSERR_NOERROR) {
        printf("error\n");
    }

    pdata->whd.lpData = data;
    pdata->whd.dwBufferLength = wi.data_size;
    pdata->whd.dwFlags = 0;
    pdata->whd.dwLoops = 1;

    waveOutPrepareHeader(pdata->wh, &(pdata->whd), sizeof(pdata->whd));
}

void Sound::SetVolume(Sound::SOUNDDATA* pdata, float volume) {
    if(pdata == NULL) {
        return;
    }
}

void Sound::SetIsLooping(Sound::SOUNDDATA *pdata, int isLooping) {
    if(pdata == NULL) {
        return;
    }
}

void Sound::Play(Sound::SOUNDDATA* pdata) {
    if(pdata == NULL) {
        return;
    }

    waveOutWrite(pdata->wh, &(pdata->whd), sizeof(pdata->whd));
}

void Sound::Pause(Sound::SOUNDDATA *pdata) {
    if(pdata == NULL) {
        return;
    }
}

void Sound::Stop(Sound::SOUNDDATA *pdata) {
    if(pdata == NULL) {
        return;
    }
}

void Sound::DestroySoundData(Sound::SOUNDDATA* pdata) {
    if(pdata == NULL) {
        return;
    }

    waveOutClose(pdata->wh);
}
