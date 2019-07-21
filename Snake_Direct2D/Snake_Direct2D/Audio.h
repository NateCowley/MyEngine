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

/// <summary>
/// The audio section of the engine. It is designed as a singleton.
/// </summary>
class Audio
{
public:
	
	/// <summary>
	/// Retrieve the instance for use
	/// </summary>
	static Audio* getInstance();
	~Audio();

	/// <summary>
	/// Create the Audio object. If it returns false, the Audio section has failed, and cannot be used.
	/// </summary>
	bool Init();

	/// <summary>
	/// Cleanup the Audio object. Must be called before deleting the object.
	/// </summary>
	void Shutdown();

	/// <summary>
	/// Retrieve the IXAudio2 device for use elsewhere
	/// </summary>
	IXAudio2* getDevice();

	/// <summary>
	/// Disable audio at the source
	/// </summary>
	void SuspendAudio();

	/// <summary>
	/// Resume audio at the source
	/// </summary>
	void ResumeAudio();

private:

	static Audio* instance;
	Audio();

	/// <summary>
	/// Check if the audio engine is available. Internal use only
	/// </summary>
	bool audioIsAvailable;

	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasterVoice;
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };
	IMFAttributes* sourceReaderConfiguration;
};