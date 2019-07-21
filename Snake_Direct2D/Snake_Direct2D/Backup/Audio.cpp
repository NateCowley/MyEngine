#include "Audio.h"

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

Audio* Audio::instance = NULL;

Audio* Audio::getInstance()
{
	if (instance == NULL)
	{
		instance = new Audio();
	}

	return instance;
}

Audio::~Audio()
{

}

void Audio::Shutdown()
{
	MFShutdown();

	if (pMasterVoice)
	{
		pMasterVoice->DestroyVoice();
	}

	if (pXAudio2)
	{
		pXAudio2->StopEngine();
		pXAudio2->Release();
	}
}

Audio::Audio()
{
	pXAudio2 = NULL;
	pMasterVoice = NULL;
	audioIsAvailable = false;
}

bool Audio::Init()
{
	if (audioIsAvailable || pXAudio2 || pMasterVoice)
	{
		return false;
	}

	HRESULT hr;

	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		return false;
	}

	pMasterVoice = NULL;
	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
	{
		return false;
	}

	hr = MFStartup(MF_VERSION);
	if (FAILED(hr))
	{
		return false;
	}

	hr = MFCreateAttributes(&sourceReaderConfiguration, 1);
	if (FAILED(hr))
	{
		return false;
	}

	hr = sourceReaderConfiguration->SetUINT32(MF_LOW_LATENCY, true);
	if (FAILED(hr))
	{
		return false;
	}

	audioIsAvailable = true;
	return true;
}

IXAudio2* Audio::getDevice()
{
	return pXAudio2;
}

void Audio::SuspendAudio()
{
	if (audioIsAvailable)
	{
		pXAudio2->StopEngine();
	}
}

void Audio::ResumeAudio()
{
	if (audioIsAvailable)
	{
		pXAudio2->StartEngine();
	}
}