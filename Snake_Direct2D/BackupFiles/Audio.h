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

	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD &dwChunkSize, DWORD &dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *buffer, DWORD buffersize, DWORD bufferoffset);
	HRESULT OpenFile(TCHAR *strFileName);
	HRESULT OpenResourceFile(const char* lpName, int lpType);

	bool loadFile(const char* fileName, std::vector<BYTE>& audioData, WAVEFORMATEX** waveFormatEx, unsigned int& waveFormatLength);
	IXAudio2* getDevice();
	
	void SuspendAudio();
	void ResumeAudio();

private:

	static Audio* instance;
	Audio();

	bool audioIsAvailable;

	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasterVoice;
	IXAudio2SourceVoice* pSourceVoice;
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };
	IMFAttributes* sourceReaderConfiguration;
};