#ifndef BGM_H
#define BGM_H

#include <xaudio2.h>
#include <x3daudio.h>

#include <string>

class AudioManager
{
public:
    void Init();

    void RunBGMAudio();

    void RunBoosterAudio(bool isPlayer1);

    void RunHasshaAudio(bool isPlayer1);

    void RunTyakudanAudio(bool isPlayer1);

    void Run();

    void ShutDown();

    void Update();

    void SetListenerPos(const DirectX::XMFLOAT3 &pos)
    {
        listenerPos = pos;
    }

    void SetPlayer1Pos(const DirectX::XMFLOAT3 &pos)
    {
        player1Pos = pos;
    }

    void SetPlayer2Pos(const DirectX::XMFLOAT3 &pos)
    {
        player2Pos = pos;
    }

private:
    struct WaveData
    {
        WAVEFORMATEX waveFormat;
        char *data;
        DWORD dataSize;

        ~WaveData()
        {
            delete[] data;
        }
    };

    IXAudio2 *pXAudio = nullptr;
    IXAudio2MasteringVoice *pMasteringVoice = nullptr;

    IXAudio2SourceVoice *pBgmSourceVoice = nullptr;
    WaveData bgmWaveData = {};

    IXAudio2SourceVoice *pBoosterSourceVoice = nullptr;
    WaveData boosterWaveData = {};

    IXAudio2SourceVoice *pHasshaSourceVoice = nullptr;
    WaveData hasshaWaveData = {};

    IXAudio2SourceVoice *pTyakudanSourceVoice = nullptr;
    WaveData tyakudanWaveData = {};

    IXAudio2SourceVoice *pPlayer2BoosterSourceVoice = nullptr;
    WaveData player2BoosterWaveData = {};

    IXAudio2SourceVoice *pPlayer2HasshaSourceVoice = nullptr;
    WaveData player2HasshaWaveData = {};

    IXAudio2SourceVoice *pPlayer2TyakudanSourceVoice = nullptr;
    WaveData player2TyakudanWaveData = {};

    X3DAUDIO_HANDLE x3dInstance = {};
    DWORD channelMask = 0;
    UINT32 channelCount = 0;
    X3DAUDIO_LISTENER listener = {};
    X3DAUDIO_DSP_SETTINGS dspSettings = {};

    X3DAUDIO_EMITTER player1Emitter = {};
    X3DAUDIO_EMITTER player2Emitter = {};

    XAUDIO2_BUFFER hasshaBuffer = {};
    XAUDIO2_BUFFER tyakudanBuffer = {};

    DirectX::XMFLOAT3 listenerPos = {};

    DirectX::XMFLOAT3 player1Pos = {};
    DirectX::XMFLOAT3 player2Pos = {};

    FLOAT32 matrixCoefficients[2 * 8] = {};

    HRESULT LoadWaveFile(const std::string &filePath, WaveData *out);
};

#endif  // BGM_H
