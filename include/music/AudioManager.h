#ifndef BGM_H
#define BGM_H

#include <xaudio2.h>
#include <x3daudio.h>

#include <string>

class AudioManager
{
public:
    void Run();
    void ShutDown();

private:
    struct WaveData
    {
        WAVEFORMATEX waveFormat;
        char* data;
        DWORD dataSize;

        ~WaveData()
        {
            delete[] data;
        }
    };

    IXAudio2* pXAudio = nullptr;
    IXAudio2MasteringVoice* pMasteringVoice = nullptr;

    IXAudio2SourceVoice* pBgmSourceVoice = nullptr;
    WaveData bgmWaveData = {};

    IXAudio2SourceVoice* pBoosterSourceVoice = nullptr;
    WaveData boosterWaveData = {};

    IXAudio2SourceVoice* pHasshaSourceVoice = nullptr;
    WaveData hasshaWaveData = {};

    IXAudio2SourceVoice* pTyakudanSourceVoice = nullptr;
    WaveData tyakudanWaveData = {};

    X3DAUDIO_HANDLE x3dInstance = {};
    DWORD channelMask = 0;
    UINT32 channelCount = 0;
    X3DAUDIO_LISTENER listener = {};

    HRESULT LoadWaveFile(const std::string& filePath, WaveData* out);
};

#endif  // BGM_H
