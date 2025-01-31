#include "AudioSource.h"
#include "../Assets/Objects/AudioAsset.h"

#include "../Assets/AssetManager.h"
#include "../Assets/AssetCatalog.h"

using namespace std;
using namespace ZEngine;

AudioSource::AudioSource() : Component("AudioSource", AUDIO_SOURCE)
{
	RegisterDerivedType(AUDIO_SOURCE);
	_audioAsset = nullptr;
	_playingChannel = -1;
	_autoPlay = false;
	_hasAutoPlayed = false;
}

void AudioSource::SetAudio(AudioAsset* asset)
{
	_audioAsset = asset;
}

AudioAsset* AudioSource::GetAudio() const
{
	return _audioAsset;
}

void AudioSource::SetAutoPlay(bool autoPlay)
{
	_autoPlay = autoPlay;
}

bool AudioSource::IsAutoPlay() const
{
	return _autoPlay;
}

void AudioSource::Play()
{
	if (_audioAsset == nullptr)
		return;

	auto audioSys = AudioSystem::GetInstance();
	_playingChannel = audioSys->Play(_audioAsset->GetSound());
}

void AudioSource::Pause()
{
	if (_audioAsset == nullptr)
		return;

	auto audioSys = AudioSystem::GetInstance();
	
	if (_audioAsset->GetSound().chunk != nullptr)
	{
		audioSys->Pause(_playingChannel);
	}
	else if (_audioAsset->GetSound().music != nullptr)
	{
		audioSys->PauseMusic();
	}
}

void AudioSource::Stop()
{
	if (_audioAsset == nullptr)
		return;

	auto audioSys = AudioSystem::GetInstance();

	if (_audioAsset->GetSound().chunk != nullptr)
		audioSys->Stop(_playingChannel);
	else if (_audioAsset->GetSound().music != nullptr)
		audioSys->StopMusic();
}

void AudioSource::Init()
{

}

void AudioSource::Update()
{
	if (!_hasAutoPlayed && IsAutoPlay())
	{
		Play();
		_hasAutoPlayed = true;
	}
}

void AudioSource::Render(int viewId)
{
}

AudioSource::~AudioSource()
{
}

ZObject* AudioSource::CreateInstance(string name, ObjectType type)
{
	return new AudioSource();
}

ZObject* AudioSource::Copy(string name, ZObject* object)
{
	if (object == nullptr || object->GetType() != AUDIO_SOURCE)
		return nullptr;

	auto source = static_cast<AudioSource*>(object);
	auto copy = new AudioSource();
	
	copy->SetAutoPlay(source->IsAutoPlay());
	copy->SetAudio(source->GetAudio());

	return copy;
}
