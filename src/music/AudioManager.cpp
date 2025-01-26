#include "music/AudioManager.h"

void AudioManager::Run()
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

    WaveData waveData = {};
    hr = LoadWaveFile("resources/music/hackathon3_r2.wav", &waveData);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: LoadWaveFile failed\n");
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        CoUninitialize();
        return;
    }

    WAVEFORMATEX waveFormat;
    memcpy(&waveFormat, &waveData.waveFormat, sizeof(WAVEFORMATEX));
    waveFormat.wBitsPerSample = waveFormat.nBlockAlign * 8 / waveFormat.nChannels;

    hr = pXAudio->CreateSourceVoice(&pBgmSourceVoice, &waveFormat);
    if (FAILED(hr))
    {
        OutputDebugString("FATAL: CreateSourceVoice failed\n");
        delete &waveData;
        pMasteringVoice->DestroyVoice();
        pXAudio->Release();
        return;
    }

    XAUDIO2_BUFFER buffer = {};
    buffer.AudioBytes = waveData.dataSize;
    buffer.pAudioData = reinterpret_cast<BYTE*>(waveData.data);
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

    hr = pBgmSourceVoice->Start(0);
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
            break;
        }
    }
}

void AudioManager::ShutDown()
{
    pBgmSourceVoice->DestroyVoice();
    pMasteringVoice->DestroyVoice();
    pXAudio->Release();
}

HRESULT AudioManager::LoadWaveFile(const std::string& filePath, WaveData* out)
{
    if (out)
    {
        out->data = nullptr;
        out->dataSize = 0;
    }
    else
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