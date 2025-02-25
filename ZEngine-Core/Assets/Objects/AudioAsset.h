#pragma once

#include "../Asset.h"
#include "../../Utilities/JsonHelpers.h"
#include "../../Audio/AudioSystem.h"

namespace ZEngine 
{
	class AudioAsset : public Asset
	{
	public:
		AudioAsset(std::string name);
		~AudioAsset();

		bool Load(std::string path) override;
		void Release() override;

		const Sound& GetSound() const;

	private:
		Sound _sound;

	public:
		static ZObject* CreateInstance(std::string name, ObjectType type);

		static ObjectType GetStaticType()
		{
			return AUDIO_ASSET;
		}
	};
}
