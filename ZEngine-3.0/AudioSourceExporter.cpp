#include "AudioSourceExporter.h"
#include <ZEngine-Core/Component/AudioSource.h>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Assets/Objects/AudioAsset.h>

using namespace nlohmann;

json AudioSourceExporter::ExportImpl(ZObject* obj)
{
	auto audioSource = static_cast<AudioSource*>(obj);
	auto catalog = AssetManager::GetInstance()->GetCatalog();

	json result;

	result["name"] = audioSource->GetName();
	result["type"] = audioSource->GetType();
	
	int audioId = catalog != nullptr ? catalog->GetAssetIDFromPath(audioSource->GetAudio()->GetPath()) : -1;

	if (audioId < 0)
		return json();

	result["audio"] = audioId;
	result["autoplay"] = audioSource->IsAutoPlay();

	return result;
}