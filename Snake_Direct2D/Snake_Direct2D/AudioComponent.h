#pragma once

/*
AudioComponent is basically an expanded IXAudio2SourceVoice made to work 
in tandem with an AudioPlayer object. The AudioComponent locks in the source 
file and streams the data. 
*/

#pragma comment(lib, "Xaudio2.lib")

#include <xaudio2.h>

#include "Audio.h"
#include "EngineTools.h"

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

/// <summary>
/// The AudioComponent class holds a reference to a .wav file and 
/// provides the source voice to be supplied to an AudioPlayer.
/// </summary>
class AudioComponent
{
public:
	/// <summary>
	/// Default constructor.
	/// </summary>
	AudioComponent();

	/// <summary>
	/// Most common constructor, supply it with the path to a .wav file.
	/// </summary>
	AudioComponent(TCHAR *strFileName);

	/// <summary>
	/// Unfinished constructor, will eventually allow the AudioComponent 
	/// to create a source voice from a built in resource.
	/// </summary>
	AudioComponent(const char* lpName, int lpType);

	/// <summary>
	/// Destructor
	/// </summary>
	~AudioComponent();

	/// <summary>
	/// Returns whether or not the AudioComponent is ready to play.
	/// </summary>
	bool isAvailable();

private:

	// the AudioPlayer and Audio classes need access to certain private 
	// members of the AudioComponent class
	friend class AudioPlayer;
	friend class Audio;

	/// <summary>
	/// Opens a file using the given path. Upon completion, sets the 
	/// private member 'available' to true.
	/// </summary>
	HRESULT OpenFile(TCHAR *strFileName);

	/// <summary>
	/// Unfinished, will be used to open a resource file.
	/// </summary>
	HRESULT OpenResourceFile(const char* lpName, int lpType);

	/// <summary>
	/// Used in opening the file.
	/// </summary>
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD &dwChunkSize, DWORD &dwChunkDataPosition);

	/// <summary>
	/// Used in opening the file.
	/// </summary>
	HRESULT ReadChunkData(HANDLE hFile, void *buffer, DWORD buffersize, DWORD bufferoffset);

	/// <summary>
	/// Returns the XAUDIO2_BUFFER necessary for the AudioPlayer to manipulate 
	/// the file.
	/// </summary>
	XAUDIO2_BUFFER* getBuffer();

	// private members
	IXAudio2SourceVoice* pSourceVoice;
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };

	bool available = false;
};