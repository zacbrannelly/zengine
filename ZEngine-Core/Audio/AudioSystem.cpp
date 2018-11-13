#include "AudioSystem.h"

#include <iostream>
#include <SDL.h>

using namespace std;

bool AudioSystem::Init()
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		cout << "AUDIO_SYSTEM: Failed to initialize SDL because: " << SDL_GetError() << endl;
		return false;
	}

	if (Mix_Init(MIX_INIT_MP3) < 0)
	{
		cout << "AUDIO_SYSTEM: Failed to initialize the mixer because: " << Mix_GetError() << endl;
		return false;
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0)
	{
		cout << "AUDIO_SYSTEM: Failed to open audio device because: " << Mix_GetError() << endl;
		return false;
	}

	return true;
}

Sound AudioSystem::LoadSoundData(string path)
{
	Sound sound;
	sound.chunk = Mix_LoadWAV(path.c_str());

	if (sound.chunk == nullptr)
	{
		cout << "AUDIO_SYSTEM: Failed to load audio data from '" << path << "' because: " << Mix_GetError() << endl;
	}

	return sound;
}

Sound AudioSystem::LoadMusicData(string path)
{
	Sound sound;
	sound.music = Mix_LoadMUS(path.c_str());

	if (sound.music == nullptr)
	{
		cout << "AUDIO_SYSTEM: Failed to load audio data from '" << path << "' because: " << Mix_GetError() << endl;
	}

	return sound;
}

void AudioSystem::SetVolume(int volume)
{
	SetVolume(-1, volume);
}

void AudioSystem::SetVolume(int channel, int volume)
{
	Mix_Volume(channel, volume);
}

int AudioSystem::Play(const Sound& sound)
{
	if (sound.chunk != nullptr)
	{
		sound.chunk->volume = sound.volume;

		if (sound.fadeIn)
			return Mix_FadeInChannel(sound.channel, sound.chunk, sound.loops, sound.fadeTime);
		else
			return Mix_PlayChannel(sound.channel, sound.chunk, sound.loops);
	}
	else if (sound.music != nullptr)
	{
		Mix_VolumeMusic(sound.volume);

		if (sound.fadeIn)
			Mix_FadeInMusic(sound.music, sound.loops, sound.fadeTime);
		else
			Mix_PlayMusic(sound.music, sound.loops);
	}

	return -1;
}

void AudioSystem::Pause(int channel)
{
	if (!Mix_Paused(channel))
		Mix_Pause(channel);
}

void AudioSystem::Resume(int channel)
{
	if (Mix_Paused(channel))
		Mix_Resume(channel);
}

void AudioSystem::PauseMusic()
{
	if (!Mix_PausedMusic())
		Mix_PauseMusic();
}

void AudioSystem::ResumeMusic()
{
	if (Mix_PausedMusic())
		Mix_ResumeMusic();
}

void AudioSystem::Stop()
{
	Stop(-1);
}

void AudioSystem::Stop(int channel)
{
	Mix_HaltChannel(channel);
}

void AudioSystem::Stop(int channel, int fadeOutTime)
{
	Mix_FadeOutChannel(channel, fadeOutTime);
}

void AudioSystem::StopMusic()
{
	Mix_HaltMusic();
}

void AudioSystem::Destroy(Sound& sound)
{
	if (sound.chunk != nullptr)
		Mix_FreeChunk(sound.chunk);

	if (sound.music != nullptr)
		Mix_FreeMusic(sound.music);
}

void AudioSystem::Shutdown()
{
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}
