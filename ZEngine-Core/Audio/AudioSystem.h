#pragma once

#include <string>
#include <SDL_mixer.h>
#include "../Misc/Singleton.h"

struct Sound
{
	Sound() : channel(-1), chunk(nullptr), music(nullptr), fadeIn(false), fadeTime(0), loops(0), volume(128)
	{
	}

	std::string name;
	int channel;
	int volume;
	bool fadeIn;
	int fadeTime;
	int loops;
	Mix_Chunk* chunk;
	Mix_Music* music;
};

class AudioSystem : public Singleton<AudioSystem>
{
public:
	bool Init();
	void Shutdown();

	void SetVolume(int volume);
	void SetVolume(int channel, int volume);

	Sound LoadSoundData(std::string path);
	Sound LoadMusicData(std::string path);

	void Destroy(Sound& sound);

	int Play(const Sound& sound);
	void Pause(int channel);
	void Resume(int channel);
	void PauseMusic();
	void ResumeMusic();
	void Stop();
	void Stop(int channel);
	void Stop(int channel, int fadeOutTime);
	void StopMusic();
};

