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
	pSourceVoice = NULL;
	audioIsAvailable = false;
}

bool Audio::Init()
{
	if (audioIsAvailable || pXAudio2 || pMasterVoice || pSourceVoice)
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

bool Audio::loadFile(const char* fileName, std::vector<BYTE>& audioData, WAVEFORMATEX** waveFormatEx, unsigned int& waveFormatLength)
{
	HRESULT hr = S_OK;

	IMFSourceReader* sourceReader;
	
	hr = MFCreateSourceReaderFromURL((LPCWSTR)fileName, sourceReaderConfiguration, &sourceReader);
	if (FAILED(hr))
	{
		return false;
	}

	DWORD streamIndex = (DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM;

	hr = sourceReader->SetStreamSelection((DWORD)MF_SOURCE_READER_ALL_STREAMS, false);
	if (FAILED(hr))
	{
		return false;
	}

	hr = sourceReader->SetStreamSelection(streamIndex, true);
	if (FAILED(hr))
	{
		return false;
	}

	IMFMediaType* nativeMediaType;

	hr = sourceReader->GetNativeMediaType(streamIndex, 0, &nativeMediaType);
	if (FAILED(hr))
	{
		return false;
	}

	GUID majorType{};
	hr = nativeMediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
	if (FAILED(hr) || majorType != MFMediaType_Audio)
	{
		return false;
	}

	GUID subType{};
	hr = nativeMediaType->GetGUID(MF_MT_MAJOR_TYPE, &subType);
	if (FAILED(hr))
	{
		return false;
	}

	if (subType == MFAudioFormat_Float || subType == MFAudioFormat_PCM)
	{
		// the audio file is uncompressed
	}
	else
	{
		// the audio file is compressed; we have to decompress it first
		IMFMediaType* partialType = NULL;
		hr = MFCreateMediaType(&partialType);
		if (FAILED(hr))
		{
			return false;
		}

		hr = partialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		if (FAILED(hr))
		{
			return false;
		}

		hr = partialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
		if (FAILED(hr))
		{
			return false;
		}

		hr = sourceReader->SetCurrentMediaType(streamIndex, NULL, partialType);
		if (FAILED(hr))
		{
			return false;
		}
	}

	// uncompress the data

	IMFMediaType* uncompressedAudioType = NULL;
	hr = sourceReader->GetCurrentMediaType(streamIndex, &uncompressedAudioType);
	if (FAILED(hr))
	{
		return false;
	}

	hr = MFCreateWaveFormatExFromMFMediaType(uncompressedAudioType, waveFormatEx, &waveFormatLength);
	if (FAILED(hr))
	{
		return false;
	}

	hr = sourceReader->SetStreamSelection(streamIndex, true);
	if (FAILED(hr))
	{
		return false;
	}

	IMFSample* sample;
	IMFMediaBuffer* buffer;
	BYTE* localAudioData;
	DWORD localAudioDataLength = 0;

	while (true)
	{
		DWORD flags = 0;
		hr = sourceReader->ReadSample(streamIndex, 0, NULL, &flags, NULL, &sample);
		if (FAILED(hr))
		{
			return false;
		}

		if (flags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
		{
			break;
		}

		if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			break;
		}

		if (sample == NULL)
		{
			continue;
		}

		// convert data to a contiguous buffer
		hr = sample->ConvertToContiguousBuffer(&buffer);
		if (FAILED(hr))
		{
			return false;
		}

		// lock buffer and copy data to local memory
		hr = buffer->Lock(&localAudioData, NULL, &localAudioDataLength);
		if (FAILED(hr))
		{
			return false;
		}

		for (size_t i = 0; i < localAudioDataLength; i++)
		{
			audioData.push_back(localAudioData[i]);
		}

		hr = buffer->Unlock();
		localAudioData = NULL;

		if (FAILED(hr))
		{
			return false;
		}
	}

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

HRESULT Audio::FindChunk(HANDLE hFile, DWORD fourcc, DWORD &dwChunkSize, DWORD &dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;

}

HRESULT Audio::ReadChunkData(HANDLE hFile, void *buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

HRESULT Audio::OpenFile(TCHAR *strFileName)
{
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (INVALID_HANDLE_VALUE == hFile || INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
	{
		return S_FALSE;
	}

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	buffer.AudioBytes = dwChunkSize;
	buffer.pAudioData = pDataBuffer;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	buffer.PlayBegin = OFFSET_SECONDS(144);

	HRESULT hr = S_OK;
	if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx)))
	{
		_com_error err(hr);
		MessageBox(NULL, (LPCSTR)err.ErrorMessage(), "", MB_OK);
		return false;
	}
	
	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
	{
		return hr;
	}
	
	if (FAILED(hr = pSourceVoice->Start(0)))
	{
		return hr;
	}

	return S_OK;
}

HRESULT Audio::OpenResourceFile(const char* lpName, int lpType)
{


	return S_OK;
}