#include "AudioSource.h"
#include "../Assets/Objects/AudioAsset.h"
#include "../Scripting/ScriptSystem.h"

using namespace std;

AudioSource::AudioSource() : Component("AudioSource", AUDIO_SOURCE)
{
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
	if (object->GetType() == AUDIO_SOURCE)
		return nullptr;

	auto source = static_cast<AudioSource*>(object);
	auto copy = new AudioSource();

	copy->SetAudio(source->GetAudio());

	return copy;
}

void Callback_AudioSource_Play(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto wrap = v8::Local<v8::External>::Cast(info.Holder()->ToObject(info.GetIsolate())->GetInternalField(0));
	auto scriptable = static_cast<IScriptable*>(wrap->Value());
	auto audioSource = static_cast<AudioSource*>(scriptable);

	if (audioSource == nullptr)
		return;

	audioSource->Play();
}

void Callback_AudioSource_Pause(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto wrap = v8::Local<v8::External>::Cast(info.Holder()->ToObject(info.GetIsolate())->GetInternalField(0));
	auto scriptable = static_cast<IScriptable*>(wrap->Value());
	auto audioSource = static_cast<AudioSource*>(scriptable);

	if (audioSource == nullptr)
		return;

	audioSource->Pause();
}

void Callback_AudioSource_Stop(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto wrap = v8::Local<v8::External>::Cast(info.Holder()->ToObject(info.GetIsolate())->GetInternalField(0));
	auto scriptable = static_cast<IScriptable*>(wrap->Value());
	auto audioSource = static_cast<AudioSource*>(scriptable);

	if (audioSource == nullptr)
		return;

	audioSource->Stop();
}

v8::Global<v8::FunctionTemplate> AudioSource::GetTemplate(v8::Isolate* isolate)
{
	using namespace v8;

	auto sys = ScriptSystem::GetInstance();

	auto temp = FunctionTemplate::New(sys->GetIsolate());

	temp->InstanceTemplate()->SetInternalFieldCount(1);
	temp->InstanceTemplate()->Set(sys->GetIsolate(), "Play", FunctionTemplate::New(sys->GetIsolate(), Callback_AudioSource_Play));
	temp->InstanceTemplate()->Set(sys->GetIsolate(), "Pause", FunctionTemplate::New(sys->GetIsolate(), Callback_AudioSource_Pause));
	temp->InstanceTemplate()->Set(sys->GetIsolate(), "Stop", FunctionTemplate::New(sys->GetIsolate(), Callback_AudioSource_Stop));

	return Global<FunctionTemplate>(sys->GetIsolate(), temp);
}
