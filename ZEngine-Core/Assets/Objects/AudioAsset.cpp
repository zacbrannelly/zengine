#include "AudioAsset.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <SDL_mixer.h>

#include "../../Audio/AudioSystem.h"
#include "../../Utilities/Directory.h"
#include "../../Logging/LoggingSystem.h"

using namespace std;
using namespace nlohmann;
using namespace ZEngine;

static const string MODULE_NAME = "AudioAsset";

AudioAsset::AudioAsset(string name) : Asset(name, AUDIO_ASSET)
{
	RegisterDerivedType(AUDIO_ASSET);
}

ZObject* AudioAsset::CreateInstance(string name, ObjectType type)
{
	return new AudioAsset(name);
}

bool AudioAsset::Load(string path)
{
	ifstream in(path, ios::in);

	if (!in.is_open())
	{
		LoggingSystem::GetInstance()->LogError("Load: Failed to load from file: " + path, MODULE_NAME);
		return false;
	}

	json root;
	root << in;

	auto audioSys = AudioSystem::GetInstance();

	auto assetDir = Directory::GetBasePath(path);
	if (assetDir[assetDir.size() - 1] != '/')
		assetDir += '/';

	// Load music data using the audio system.
	auto it = root.find("music");
	if (it != root.end())
	{
		auto soundPath = assetDir + it->get<string>();
		_sound = audioSys->LoadMusicData(soundPath);
	}

	// Load sound data using the audio system.
	it = root.find("sound");
	if (it != root.end())
	{
		auto soundPath = assetDir + it->get<string>();
		_sound = audioSys->LoadSoundData(soundPath);
	}

	if (_sound.chunk == nullptr && _sound.music == nullptr)
	{
		LoggingSystem::GetInstance()->LogError("Load: Failed to load the asset " + GetName(), MODULE_NAME);
		return false;
	}
	
	// Inject the JSON props into the Sound instance.
	root.get_to(_sound);

	SetPath(path);
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
