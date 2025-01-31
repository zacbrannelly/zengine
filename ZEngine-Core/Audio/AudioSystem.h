#pragma once

#include <string>
#include <SDL_mixer.h>
#include "../Utilities/JsonHelpers.h"
#include "../Misc/Singleton.h"

namespace ZEngine
{
	struct Sound
	{
	public:
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

		JSON_SCHEMA_BEGIN(Sound)
			JSON_MAP_TO_MEMBER_OPTIONAL(name, name)
			JSON_MAP_TO_MEMBER_OPTIONAL(channel, channel)
			JSON_MAP_TO_MEMBER_OPTIONAL(volume, volume)
			JSON_MAP_TO_MEMBER_OPTIONAL(fadeIn, fadeIn)
			JSON_MAP_TO_MEMBER_OPTIONAL(fadeTime, fadeTime)
			JSON_MAP_TO_MEMBER_OPTIONAL(loops, loops)
		JSON_SCHEMA_END()
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
}
