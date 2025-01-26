#include "music/AudioManager.h"

void AudioManager::Init()
{
    HRESULT hr = XAudio2Create(&pXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: XAudio2Create failed\n");
        CoUninitialize();
        return;
    }

    hr = pXAudio->CreateMasteringVoice(&pMasteringVoice);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: CreateMasteringVoice failed\n");
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    XAUDIO2_DEBUG_CONFIGURATION debugConfig = {};
    debugConfig.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
    debugConfig.BreakMask = XAUDIO2_LOG_ERRORS;
    pXAudio->SetDebugConfiguration(&debugConfig);

    XAUDIO2_VOICE_DETAILS voiceDetails = {};
    pMasteringVoice->GetVoiceDetails(&voiceDetails);

    if (voiceDetails.InputChannels > 8)
    {
        OutputDebugString("fatal: invalid number of output channels\n");
        return;
    }
    channelCount = voiceDetails.InputChannels;
    hr = pMasteringVoice->GetChannelMask(&channelMask);
    if (FAILED(hr))
    {
        OutputDebugString("fatal: GetChannelMask failed\n");
        return;
    }

    hr = X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, x3dInstance);
    if (FAILED(hr))
    {
        OutputDebugString("fatal: X3DAudioInitialize failed\n");
        return;
    }

    listenerPos = {0.0f, 0.0f, 0.0f};
    player1Pos = {0.0f, 0.0f, 0.0f};
    player2Pos = {0.0f, 0.0f, 0.0f};
    listener.Position.x = listenerPos.x;
    listener.Position.y = listenerPos.y;
    listener.Position.z = listenerPos.z;
    listener.OrientFront = {0.0f, 0.0f, 1.0f};
    listener.OrientTop = {0.0f, 1.0f, 0.0f};
    listener.Velocity = {0.0f, 0.0f, 0.0f};

    player1Emitter.Position.x = player1Pos.x;
    player1Emitter.Position.y = player1Pos.y;
    player1Emitter.Position.z = player1Pos.z;
    player1Emitter.OrientFront = {0.0f, 0.0f, 1.0f};
    player1Emitter.OrientTop = {0.0f, 1.0f, 0.0f};
    player1Emitter.Velocity = {0.0f, 0.0f, 0.0f};
    player1Emitter.InnerRadius = 100.0f;
    player1Emitter.InnerRadiusAngle = X3DAUDIO_2PI;
    player1Emitter.ChannelCount = 2;
    player1Emitter.ChannelRadius = 1.0f;
    player1Emitter.CurveDistanceScaler = 100.0f;
    player1Emitter.DopplerScaler = 1.0f;
    FLOAT32 *player1EmitterAzimuths = new FLOAT32[2];
    player1EmitterAzimuths[0] = 0.0f;
    player1EmitterAzimuths[1] = X3DAUDIO_2PI;
    player1Emitter.pChannelAzimuths = player1EmitterAzimuths;

    player2Emitter.Position.x = player2Pos.x;
    player2Emitter.Position.y = player2Pos.y;
    player2Emitter.Position.z = player2Pos.z;
    player2Emitter.OrientFront = {0.0f, 0.0f, 1.0f};
    player2Emitter.OrientTop = {0.0f, 1.0f, 0.0f};
    player2Emitter.Velocity = {0.0f, 0.0f, 0.0f};
    player2Emitter.InnerRadius = 100.0f;
    player2Emitter.InnerRadiusAngle = X3DAUDIO_2PI;
    player2Emitter.ChannelCount = 2;
    player2Emitter.ChannelRadius = 1.0f;
    player2Emitter.CurveDistanceScaler = 100.0f;
    player2Emitter.DopplerScaler = 1.0f;
    FLOAT32 *player2EmitterAzimuths = new FLOAT32[2];
    player2EmitterAzimuths[0] = 0.0f;
    player2EmitterAzimuths[1] = X3DAUDIO_2PI;
    player2Emitter.pChannelAzimuths = player2EmitterAzimuths;

    dspSettings.SrcChannelCount = 2;
    dspSettings.DstChannelCount = channelCount;
    dspSettings.pMatrixCoefficients = matrixCoefficients;

    hr = LoadWaveFile("resources/music/hackathon3_r2.wav", &bgmWaveData);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: LoadWaveFile failed\n");
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    hr = LoadWaveFile("resources/music/booster.wav", &boosterWaveData);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: LoadWaveFile failed\n");
        delete &bgmWaveData;
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    hr = LoadWaveFile("resources/music/hassya.wav", &hasshaWaveData);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: LoadWaveFile failed\n");
        delete &bgmWaveData;
        delete &boosterWaveData;
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    hr = LoadWaveFile("resources/music/tyakudan.wav", &tyakudanWaveData);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: LoadWaveFile failed\n");
        delete &bgmWaveData;
        delete &boosterWaveData;
        delete &hasshaWaveData;
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    WAVEFORMATEX waveFormat;
    memcpy(&waveFormat, &bgmWaveData.waveFormat, sizeof(WAVEFORMATEX));
    waveFormat.wBitsPerSample = waveFormat.nBlockAlign * 8 / waveFormat.nChannels;

    hr = pXAudio->CreateSourceVoice(&pBgmSourceVoice, &waveFormat);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: CreateSourceVoice failed\n");
        delete &bgmWaveData;
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        return;
    }

    XAUDIO2_BUFFER buffer = {};
    buffer.AudioBytes = bgmWaveData.dataSize;
    buffer.pAudioData = reinterpret_cast<BYTE *>(bgmWaveData.data);
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

    hr = pBgmSourceVoice->SubmitSourceBuffer(&buffer);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: SubmitSourceBuffer failed\n");
        pBgmSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    WAVEFORMATEX boosterWaveFormat;
    memcpy(&boosterWaveFormat, &boosterWaveData.waveFormat, sizeof(WAVEFORMATEX));
    boosterWaveFormat.wBitsPerSample = boosterWaveFormat.nBlockAlign * 8 / boosterWaveFormat.nChannels;

    hr = pXAudio->CreateSourceVoice(&pBoosterSourceVoice, &boosterWaveFormat);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: CreateSourceVoice failed\n");
        delete &bgmWaveData;
        pBgmSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    hr = pXAudio->CreateSourceVoice(&pPlayer2BoosterSourceVoice, &boosterWaveFormat);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: CreateSourceVoice failed\n");
        delete &bgmWaveData;
        pBgmSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    XAUDIO2_BUFFER boosterBuffer = {};
    boosterBuffer.AudioBytes = boosterWaveData.dataSize;
    boosterBuffer.pAudioData = reinterpret_cast<BYTE *>(boosterWaveData.data);
    boosterBuffer.Flags = XAUDIO2_END_OF_STREAM;
    boosterBuffer.LoopCount = XAUDIO2_LOOP_INFINITE;

    hr = pBoosterSourceVoice->SubmitSourceBuffer(&boosterBuffer);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: SubmitSourceBuffer failed\n");
        pBoosterSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    hr = pPlayer2BoosterSourceVoice->SubmitSourceBuffer(&boosterBuffer);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: SubmitSourceBuffer failed\n");
        pPlayer2BoosterSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    WAVEFORMATEX hasshaWaveFormat;
    memcpy(&hasshaWaveFormat, &hasshaWaveData.waveFormat, sizeof(WAVEFORMATEX));
    hasshaWaveFormat.wBitsPerSample = hasshaWaveFormat.nBlockAlign * 8 / hasshaWaveFormat.nChannels;

    hr = pXAudio->CreateSourceVoice(&pHasshaSourceVoice, &hasshaWaveFormat);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: CreateSourceVoice failed\n");
        delete &bgmWaveData;
        delete &boosterWaveData;
        pBgmSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    hr = pXAudio->CreateSourceVoice(&pPlayer2HasshaSourceVoice, &hasshaWaveFormat);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: CreateSourceVoice failed\n");
        delete &bgmWaveData;
        delete &boosterWaveData;
        pBgmSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    hasshaBuffer = {};
    hasshaBuffer.AudioBytes = hasshaWaveData.dataSize;
    hasshaBuffer.pAudioData = reinterpret_cast<BYTE *>(hasshaWaveData.data);
    hasshaBuffer.Flags = XAUDIO2_END_OF_STREAM;
    hasshaBuffer.LoopCount = 0;

    hr = pHasshaSourceVoice->SubmitSourceBuffer(&hasshaBuffer);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: SubmitSourceBuffer failed\n");
        pHasshaSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    hr = pPlayer2HasshaSourceVoice->SubmitSourceBuffer(&hasshaBuffer);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: SubmitSourceBuffer failed\n");
        pPlayer2HasshaSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    WAVEFORMATEX tyakudanWaveFormat;
    memcpy(&tyakudanWaveFormat, &tyakudanWaveData.waveFormat, sizeof(WAVEFORMATEX));
    tyakudanWaveFormat.wBitsPerSample = tyakudanWaveFormat.nBlockAlign * 8 / tyakudanWaveFormat.nChannels;

    hr = pXAudio->CreateSourceVoice(&pTyakudanSourceVoice, &tyakudanWaveFormat);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: CreateSourceVoice failed\n");
        delete &bgmWaveData;
        delete &boosterWaveData;
        delete &hasshaWaveData;
        pBgmSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    hr = pXAudio->CreateSourceVoice(&pPlayer2TyakudanSourceVoice, &tyakudanWaveFormat);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: CreateSourceVoice failed\n");
        delete &bgmWaveData;
        delete &boosterWaveData;
        delete &hasshaWaveData;
        pBgmSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    tyakudanBuffer = {};
    tyakudanBuffer.AudioBytes = tyakudanWaveData.dataSize;
    tyakudanBuffer.pAudioData = reinterpret_cast<BYTE *>(tyakudanWaveData.data);
    tyakudanBuffer.Flags = XAUDIO2_END_OF_STREAM;
    tyakudanBuffer.LoopCount = 0;

    hr = pTyakudanSourceVoice->SubmitSourceBuffer(&tyakudanBuffer);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: SubmitSourceBuffer failed\n");
        pTyakudanSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    hr = pPlayer2TyakudanSourceVoice->SubmitSourceBuffer(&tyakudanBuffer);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: SubmitSourceBuffer failed\n");
        pPlayer2TyakudanSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }
}

void AudioManager::RunBGMAudio()
{
    HRESULT hr = pBgmSourceVoice->Start(0);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: Start failed\n");
        pBgmSourceVoice->DestroyVoice();
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    XAUDIO2_VOICE_STATE state = {};
    while (true)
    {
        pBgmSourceVoice->GetState(&state);
        if (state.BuffersQueued == 0)
        {
            Update();
            break;
        }
    }
}

void AudioManager::RunBoosterAudio(bool isPlayer1)
{
    if (isPlayer1)
    {
        HRESULT hr = pBoosterSourceVoice->Start(0);
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: Start failed\n");
            pBoosterSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }
    } else
    {
        HRESULT hr = pPlayer2BoosterSourceVoice->Start(0);
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: Start failed\n");
            pPlayer2BoosterSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }
    }
}

void AudioManager::RunHasshaAudio(bool isPlayer1)
{
    if (isPlayer1)
    {
        HRESULT hr = pHasshaSourceVoice->Stop();
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: Stop failed\n");
            pHasshaSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }

        hr = pHasshaSourceVoice->FlushSourceBuffers();
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: FlushSourceBuffers failed\n");
            pHasshaSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }

        hr = pHasshaSourceVoice->SubmitSourceBuffer(&hasshaBuffer);
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: SubmitSourceBuffer failed\n");
            pHasshaSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }

        hr = pHasshaSourceVoice->Start(0);
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: Start failed\n");
            pHasshaSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }
    } else
    {
        HRESULT hr = pPlayer2HasshaSourceVoice->Stop();
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: Stop failed\n");
            pPlayer2HasshaSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }

        hr = pPlayer2HasshaSourceVoice->FlushSourceBuffers();
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: FlushSourceBuffers failed\n");
            pPlayer2HasshaSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }

        hr = pPlayer2HasshaSourceVoice->SubmitSourceBuffer(&hasshaBuffer);
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: SubmitSourceBuffer failed\n");
            pPlayer2HasshaSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }

        hr = pPlayer2HasshaSourceVoice->Start(0);
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: Start failed\n");
            pPlayer2HasshaSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }
    }
}

void AudioManager::RunTyakudanAudio(bool isPlayer1)
{
    if (isPlayer1)
    {
        HRESULT hr = pTyakudanSourceVoice->Stop();
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: Stop failed\n");
            pTyakudanSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }

        hr = pTyakudanSourceVoice->FlushSourceBuffers();
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: FlushSourceBuffers failed\n");
            pTyakudanSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }

        hr = pTyakudanSourceVoice->SubmitSourceBuffer(&tyakudanBuffer);
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: SubmitSourceBuffer failed\n");
            pTyakudanSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }

        hr = pTyakudanSourceVoice->Start(0);
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: Start failed\n");
            pTyakudanSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }
    } else
    {
        HRESULT hr = pPlayer2TyakudanSourceVoice->Stop();
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: Stop failed\n");
            pPlayer2TyakudanSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }

        hr = pPlayer2TyakudanSourceVoice->FlushSourceBuffers();
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: FlushSourceBuffers failed\n");
            pPlayer2TyakudanSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }

        hr = pPlayer2TyakudanSourceVoice->SubmitSourceBuffer(&tyakudanBuffer);
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: SubmitSourceBuffer failed\n");
            pPlayer2TyakudanSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }

        hr = pPlayer2TyakudanSourceVoice->Start(0);
        if (FAILED(hr))
        {
            OutputDebugString("FATAL: Start failed\n");
            pPlayer2TyakudanSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio->Release();
            CoUninitialize();
            return;
        }
    }
}

void AudioManager::Run()
{
}

void AudioManager::ShutDown()
{
    pBgmSourceVoice->DestroyVoice();
    pMasteringVoice->DestroyVoice();
    pXAudio->Release();
}

void AudioManager::Update()
{
    player1Emitter.Position.x = player1Pos.x;
    player1Emitter.Position.y = player1Pos.y;
    player1Emitter.Position.z = player1Pos.z;
    player2Emitter.Position.x = player2Pos.x;
    player2Emitter.Position.y = player2Pos.y;
    player2Emitter.Position.z = player2Pos.z;
    listener.Position.x = listenerPos.x;
    listener.Position.y = listenerPos.y;
    listener.Position.z = listenerPos.z;

    if (pBoosterSourceVoice != nullptr)
    {
        X3DAudioCalculate(
            x3dInstance,
            &listener,
            &player1Emitter,
            X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
            &dspSettings
        );

        HRESULT hr = pBoosterSourceVoice->SetFrequencyRatio(dspSettings.DopplerFactor);
        if (FAILED(hr))
        {
            OutputDebugString("warning: SetFrequencyRatio failed\n");
            return;
        }

        hr = pBoosterSourceVoice->SetOutputMatrix(
            pMasteringVoice,
            dspSettings.SrcChannelCount,
            dspSettings.DstChannelCount,
            dspSettings.pMatrixCoefficients
        );
        if (FAILED(hr))
        {
            OutputDebugString("warning: SetOutputMatrix failed\n");
            return;
        }
    }

    if (pHasshaSourceVoice != nullptr)
    {
        X3DAudioCalculate(
            x3dInstance,
            &listener,
            &player1Emitter,
            X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
            &dspSettings
        );

        HRESULT hr = pHasshaSourceVoice->SetFrequencyRatio(dspSettings.DopplerFactor);
        if (FAILED(hr))
        {
            OutputDebugString("warning: SetFrequencyRatio failed\n");
            return;
        }

        hr = pHasshaSourceVoice->SetOutputMatrix(
            pMasteringVoice,
            dspSettings.SrcChannelCount,
            dspSettings.DstChannelCount,
            dspSettings.pMatrixCoefficients
        );
        if (FAILED(hr))
        {
            OutputDebugString("warning: SetOutputMatrix failed\n");
            return;
        }
    }

    if (pTyakudanSourceVoice != nullptr)
    {
        X3DAudioCalculate(
            x3dInstance,
            &listener,
            &player1Emitter,
            X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
            &dspSettings
        );

        HRESULT hr = pTyakudanSourceVoice->SetFrequencyRatio(dspSettings.DopplerFactor);
        if (FAILED(hr))
        {
            OutputDebugString("warning: SetFrequencyRatio failed\n");
            return;
        }

        hr = pTyakudanSourceVoice->SetOutputMatrix(
            pMasteringVoice,
            dspSettings.SrcChannelCount,
            dspSettings.DstChannelCount,
            dspSettings.pMatrixCoefficients
        );
        if (FAILED(hr))
        {
            OutputDebugString("warning: SetOutputMatrix failed\n");
            return;
        }
    }
}

HRESULT AudioManager::LoadWaveFile(const std::string &filePath, WaveData *out)
{
    if (out)
    {
        out->data = nullptr;
        out->dataSize = 0;
    } else
    {
        OutputDebugString("LoadWaveFile: out is nullptr\n");
        return E_INVALIDARG;
    }

    HMMIO hmmio = nullptr;
    MMCKINFO chunkInfo = {};
    MMCKINFO riffChunkInfo = {};

    hmmio = mmioOpen(const_cast<LPSTR>(filePath.c_str()), nullptr, MMIO_READ);
    if (!hmmio)
    {
        OutputDebugString("fatal: mmioOpen failed\n");
        return HRESULT_FROM_WIN32(GetLastError());
    }

    riffChunkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');

    MMRESULT ret = mmioDescend(hmmio, &riffChunkInfo, nullptr, MMIO_FINDRIFF);
    if (ret != MMSYSERR_NOERROR)
    {
        OutputDebugString("fatal: mmioDescend to RIFF chunk failed\n");
        mmioClose(hmmio, MMIO_FHOPEN);
        return HRESULT_FROM_WIN32(ret);
    }

    chunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
    ret = mmioDescend(hmmio, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK);
    if (ret != MMSYSERR_NOERROR)
    {
        OutputDebugString("fatal: mmioDescend to fmt chunk failed\n");
        mmioClose(hmmio, MMIO_FHOPEN);
        return HRESULT_FROM_WIN32(ret);
    }

    DWORD readSize
        = mmioRead(hmmio, reinterpret_cast<HPSTR>(&out->waveFormat), sizeof(WAVEFORMATEX));
    if (readSize != sizeof(WAVEFORMATEX))
    {
        OutputDebugString("fatal: mmioRead for fmt chunk failed\n");
        mmioClose(hmmio, MMIO_FHOPEN);
        return HRESULT_FROM_WIN32(GetLastError());
    }

    if (out->waveFormat.wFormatTag != WAVE_FORMAT_PCM)
    {
        OutputDebugString("fatal: not PCM format\n");
        mmioClose(hmmio, MMIO_FHOPEN);
        return E_FAIL;
    }

    ret = mmioAscend(hmmio, &chunkInfo, 0);
    if (ret != MMSYSERR_NOERROR)
    {
        OutputDebugString("fatal: mmioAscend from fmt chunk failed\n");
        mmioClose(hmmio, MMIO_FHOPEN);
        return HRESULT_FROM_WIN32(ret);
    }

    chunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
    ret = mmioDescend(hmmio, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK);
    if (ret != MMSYSERR_NOERROR)
    {
        OutputDebugString("fatal: mmioDescend to data chunk failed\n");
        mmioClose(hmmio, MMIO_FHOPEN);
        return HRESULT_FROM_WIN32(ret);
    }

    out->dataSize = chunkInfo.cksize;

    out->data = new char[out->dataSize];
    readSize = mmioRead(hmmio, out->data, chunkInfo.cksize);
    if (readSize != chunkInfo.cksize)
    {
        OutputDebugString("fatal: mmioRead for data chunk failed\n");
        mmioClose(hmmio, MMIO_FHOPEN);
        delete[] out->data;
        return HRESULT_FROM_WIN32(GetLastError());
    }

    mmioClose(hmmio, MMIO_FHOPEN);

    return S_OK;
}
