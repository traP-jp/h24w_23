#ifndef BGM_H
#define BGM_H

#include <xaudio2.h>

#include <string>

class BGM
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
    IXAudio2SourceVoice* pSourceVoice = nullptr;

    WaveData waveData = {};

    HRESULT LoadWaveFile(const std::string& filePath, WaveData* out);
};

#endif  // BGM_H
