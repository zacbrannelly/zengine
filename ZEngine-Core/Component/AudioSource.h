#pragma once

#include "Component.h"
#include "../Assets/Objects/AudioAsset.h"
#include "../Utilities/JsonHelpers.h"

namespace ZEngine
{
	class AudioAsset;

	class AudioSource : public Component
	{
	public:
		AudioSource();
		~AudioSource();

		void Init() override;
		void Update() override;
		void Render(int viewId) override;

		void SetAudio(AudioAsset* asset);
		AudioAsset* GetAudio() const;

		void SetAutoPlay(bool autoPlay);
		bool IsAutoPlay() const;

		void Play();
		void Pause();
		void Stop();

	#ifndef SWIG
		// Allow serialization / deserialization
		JSON_SCHEMA_BEGIN(AudioSource)
			INCLUDE_ASSET_REFERENCES()
			JSON_MAP_TO_ASSET_REF_SETTER (audio,    SetAudio,    AudioAsset)
			JSON_MAP_TO_SETTER_OPTIONAL  (autoPlay, SetAutoPlay, bool)
		JSON_SCHEMA_END()
	#endif

	private:
		bool _autoPlay;
		AudioAsset* _audioAsset;
		bool _hasAutoPlayed;
		int _playingChannel;

	public:
		static ZObject* CreateInstance(std::string name, ObjectType type);
		static ZObject* Copy(std::string name, ZObject* object);

		static ObjectType GetStaticType()
		{
			return AUDIO_SOURCE;	
		}
	};
}
