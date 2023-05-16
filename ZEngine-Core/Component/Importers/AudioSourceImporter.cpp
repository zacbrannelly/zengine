#include "AudioSourceImporter.h"
#include "../AudioSource.h"
#include "../../Misc/Factory.h"
#include "../../Assets/Objects/AudioAsset.h"
#include "../../Assets/AssetManager.h"
#include "../../Assets/AssetCatalog.h"

using namespace std;
using namespace nlohmann;

ZObject* AudioSourceImporter::ImportImpl(string name, json& values)
{
	auto instance = Factory::CreateInstance<AudioSource>(name, AUDIO_SOURCE);
	auto assetManager = AssetManager::GetInstance();

	if (HasKey("audio", values))
	{
		auto audioId = ReadInt("audio", values);

		string path;
		ObjectType type;
		if (assetManager->GetCatalog()->GetAssetPathFromID(audioId, path, type))
		{
			auto audioAsset = assetManager->FindAssetFromPath(path);

			if (audioAsset == nullptr)
			{
				audioAsset = assetManager->LoadAsset(path, path, type);
			}

			if (audioAsset != nullptr)
				instance->SetAudio(audioAsset->Cast<AudioAsset>());
		}
	}

	if (HasKey("autoplay", values))
	{
		auto autoPlay = ReadBool("autoplay", values);
		instance->SetAutoPlay(autoPlay);
	}

	return instance;
}
