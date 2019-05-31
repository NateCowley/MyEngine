#include "AudioPlayer.h"

#ifndef CHECKFIRST_AUDIOPLAYER
#define CHECKFIRST_AUDIOPLAYER if(!isAvailable()) {return false;}
#endif

AudioPlayer::AudioPlayer()
{
	comp = NULL;
}

AudioPlayer::AudioPlayer(AudioComponent* ac)
{
	setAudioComponent(ac);
}

AudioPlayer::~AudioPlayer()
{
	comp = 0;
}

bool AudioPlayer::setAudioComponent(AudioComponent* ac)
{
	if (playing || available)
	{
		return false;
	}

	comp = ac;

	if (comp)
	{
		if (comp->isAvailable())
		{
			available = true;
		}
	}
}

bool AudioPlayer::isAvailable()
{
	return available && !playing;
}

bool AudioPlayer::startPlaying()
{
	if (!available)
	{
		return false;
	}

	resetBuffer();

	if (FAILED(comp->pSourceVoice->Start(0)))
	{
		return false;
	}

	playing = true;
	return true;
}

bool AudioPlayer::stopPlaying()
{
	if (!available)
	{
		return false;
	}
		

	if (FAILED(comp->pSourceVoice->Stop(0)))
	{
		return false;
	}

	playing = false;
	return true;
}

bool AudioPlayer::resumePlaying()
{
	CHECKFIRST_AUDIOPLAYER;

	if (FAILED(comp->pSourceVoice->Start(0)))
	{
		return false;
	}

	playing = true;
	return true;
}

bool AudioPlayer::playAt(float time)
{
	CHECKFIRST_AUDIOPLAYER;

	playBegin = OFFSET_SECONDS(time);

	setBuffer();
}

bool AudioPlayer::setPlayLength(float length)
{
	CHECKFIRST_AUDIOPLAYER;

	playLength = OFFSET_SECONDS(length);

	setBuffer();
}

bool AudioPlayer::setLoop(float begin, float length, int count, bool setPlayAt)
{
	CHECKFIRST_AUDIOPLAYER;

	if (count > XAUDIO2_MAX_LOOP_COUNT)
	{
		if (count != XAUDIO2_LOOP_INFINITE)
		{
			return false;
		}
	}

	if (count == 0)
	{
		loopBegin = 0;
		loopCount = 0;
		loopLength = 0;

		setBuffer();

		return true;
	}

	float tlb = loopBegin;
	float tll = loopLength;
	int tlc = loopCount;

	loopBegin = OFFSET_SECONDS(begin);
	loopLength = OFFSET_SECONDS(length);
	loopCount = count;

	if (loopBegin + loopLength <= playBegin || (loopBegin + loopLength >= playBegin + playLength && playBegin != 0 && playLength != 0))
	{
		loopBegin = tlb;
		loopLength = tll;
		loopCount = tlc;

		return false;
	}

	if (setPlayAt)
	{
		playAt(begin - 1);
		setPlayLength(length + 2);
	}

	setBuffer();

	return true;
}

bool AudioPlayer::isPlaying()
{
	return playing;
}

bool AudioPlayer::setVolume(float value)
{
	if (!available)
	{
		return false;
	}

	if (value < 0 || value > 100)
	{
		return false;
	}

	HRESULT hr;

	if (FAILED(hr = comp->pSourceVoice->SetVolume(value)))
	{
		return false;
	}

	return true;
}

bool AudioPlayer::increaseVolume(float step)
{
	if (!available)
	{
		return false;
	}

	float curVol = 0.0;
	comp->pSourceVoice->GetVolume(&curVol);

	if (curVol + step < 0 || curVol + step > 100)
	{
		return false;
	}

	HRESULT hr;

	if (FAILED(hr = comp->pSourceVoice->SetVolume(curVol + step)))
	{
		return false;
	}

	return true;
}

bool AudioPlayer::decreaseVolume(float step)
{
	if (!available)
	{
		return false;
	}

	float curVol = 0.0;
	comp->pSourceVoice->GetVolume(&curVol);

	if (curVol - step < 0 || curVol - step > 100)
	{
		return false;
	}

	HRESULT hr;

	if (FAILED(hr = comp->pSourceVoice->SetVolume(curVol + step)))
	{
		return false;
	}

	return true;
}

void AudioPlayer::resetPlayer()
{
	playBegin = 0.0;
	playLength = 0.0;
	loopBegin = 0.0;
	loopLength = 0.0;
	loopCount = 0;
}

void AudioPlayer::resetBuffer()
{
	if(comp)
	{
		if (playing)
		{
			stopPlaying();
		}

		flushBuffer();
		setBuffer();
	}
}

void AudioPlayer::setBuffer()
{
	if (comp)
	{
		comp->buffer.PlayBegin = playBegin;
		comp->buffer.PlayLength = playLength;
		comp->buffer.LoopBegin = loopBegin;
		comp->buffer.LoopLength = loopLength;
		comp->buffer.LoopCount = loopCount;

		flushBuffer();

		HRESULT hr;

		if (FAILED(hr = comp->pSourceVoice->SubmitSourceBuffer(comp->getBuffer())))
		{
			available = false;
		}
	}
}

void AudioPlayer::flushBuffer()
{
	if (!comp)
	{
		return;
	}

	HRESULT hr;

	if (FAILED(hr = comp->pSourceVoice->FlushSourceBuffers()))
	{
		available = false;
	}
}