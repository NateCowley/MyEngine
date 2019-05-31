#pragma once

#include "Audio.h"
#include "AudioComponent.h"

class AudioPlayer
{
public:
	AudioPlayer();
	AudioPlayer(AudioComponent* ac);
	~AudioPlayer();

	bool setAudioComponent(AudioComponent* ac);

	/// <summary>
	/// Returns true if the AudioComponent is loaded and not currently playing
	/// </summary>
	bool isAvailable();

	/// <summary>
	/// Beigin playing the loaded sound. Automatically checks for availability. If already playing, it will reset the sound component/buffer and restart..
	/// </summary>
	bool startPlaying();

	/// <summary>
	/// Stops playing the loaded sound.
	/// </summary>
	bool stopPlaying();

	/// <summary>
	/// Resumes playing. If not already playing, nothing happens.
	/// </summary>
	bool resumePlaying();

	/// <summary>
	/// Set the time to start playing at.
	/// </summary>
	bool playAt(float time);

	/// <summary>
	/// Set the duration to play for.
	/// </summary>
	bool setPlayLength(float length);

	/// <summary>
	/// <para>Setup a looping section of the sound.</para>Begin time, length, and loops count are all required, but you can choose<para>whether or not to play at the designated loop time.</para>
	/// </summary>
	bool setLoop(float begin, float length, int count, bool setPlayAt = true);

	/// <summary>
	/// 
	/// </summary>
	bool isPlaying();

	/// <summary>
	/// 
	/// </summary>
	bool setVolume(float value);

	/// <summary>
	/// 
	/// </summary>
	bool increaseVolume(float step = 1.0);
	
	/// <summary>
	/// 
	/// </summary>
	bool decreaseVolume(float step = 1.0);

private:
	AudioComponent* comp;
	bool available = false;
	bool playing = false;
	float playBegin = 0.0, playLength = 0.0, loopBegin = 0.0, loopLength = 0.0;
	int loopCount = 0;

	/// <summary>
	/// 
	/// </summary>
	void resetPlayer();

	/// <summary>
	/// 
	/// </summary>
	void setBuffer();

	/// <summary>
	/// 
	/// </summary>
	void resetBuffer();

	/// <summary>
	/// 
	/// </summary>
	void flushBuffer();
};