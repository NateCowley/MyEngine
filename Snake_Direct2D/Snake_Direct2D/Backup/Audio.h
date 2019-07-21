#pragma once

#pragma comment(lib, "Xaudio2.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid")

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <vector>
#include <strsafe.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <conio.h>
#include <comdef.h>

#include "EngineTools.h"

#ifndef OFFSET_SECONDS
/// <summary>
/// Used to calculate the number of seconds to offset PlayBegin
/// </summary>
#define OFFSET_SECONDS(x) 44100*x
#endif

class Audio
{
public:
	
	static Audio* getInstance();
	~Audio();

	bool Init();
	void Shutdown();

	IXAudio2* getDevice();

	void SuspendAudio();
	void ResumeAudio();

private:

	static Audio* instance;
	Audio();

	bool audioIsAvailable;

	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasterVoice;
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };
	IMFAttributes* sourceReaderConfiguration;
};