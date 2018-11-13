#include "AudioAsset.h"

#include <fstream>
#include <iostream>
#include "../../Audio/AudioSystem.h"
#include "../../../json/json.hpp"
#include <SDL_mixer.h>

using namespace std;
using namespace nlohmann;

AudioAsset::AudioAsset(string name) : Asset(name, AUDIO_ASSET)
{
}

Asset* AudioAsset::CreateInstance(string name)
{
	return new AudioAsset(name);
}

bool AudioAsset::Load(string path)
{
	ifstream in(path, ios::in);

	if (!in.is_open())
	{
		cout << "AUDIO_ASSET: Failed to load from file: " << path << endl;
		return false;
	}

	json root;
	root << in;

	auto audioSys = AudioSystem::GetInstance();

	// Load music data
	auto it = root.find("music");
	if (it != root.end())
	{
		auto path = it->get<string>();

		SetPath(path);
		_sound = audioSys->LoadMusicData(path);
	}

	// Load sound data 
	it = root.find("sound");
	if (it != root.end())
	{
		auto path = it->get<string>();

		SetPath(path);
		_sound = audioSys->LoadSoundData(it->get<string>());
	}

	if (_sound.chunk == nullptr && _sound.music == nullptr)
	{
		cout << "AUDIO_ASSET: Failed to load the asset " << GetName() << endl;
		return false;
	}
	
	// Load name 
	it = root.find("name");
	if (it != root.end())
		_sound.name = it->get<string>();

	// Load pref channel number
	it = root.find("channel");
	if (it != root.end())
		_sound.channel = it->get<int>();

	// Load number of times to loop
	it = root.find("loops");
	if (it != root.end())
		_sound.loops = it->get<int>();

	// Load the volume (0 - 128)
	it = root.find("volume");
	if (it != root.end())
		_sound.volume = it->get<int>();

	// Load whether to fade in on play
	it = root.find("fadeIn");
	if (it != root.end())
		_sound.fadeIn = it->get<bool>();

	// Load how long to fade for (in ms)
	it = root.find("fadeTime");
	if (it != root.end())
		_sound.fadeTime = it->get<int>();

	return true;
}

const Sound& AudioAsset::GetSound() const
{
	return _sound;
}

void AudioAsset::Release()
{
	AudioSystem::GetInstance()->Destroy(_sound);
}

AudioAsset::~AudioAsset()
{
}
