#include "AudioSourceExporter.h"
#include <ZEngine-Core/Component/AudioSource.h>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Assets/Objects/AudioAsset.h>

#include <uuid.h>

using namespace nlohmann;
using namespace ZEngine;

json AudioSourceExporter::ExportImpl(ZObject* obj)
{
	auto audioSource = static_cast<AudioSource*>(obj);
	auto catalog = AssetManager::GetInstance()->GetCatalog();

	json result;

	result["name"] = audioSource->GetName();
	result["type"] = audioSource->GetType();
	
	uuids::uuid audioId = catalog != nullptr ? catalog->GetAssetIDFromPath(audioSource->GetAudio()->GetPath()) : uuids::uuid();

	if (audioId.is_nil())
		return json();

	result["audio"] = uuids::to_string(audioId);
	result["autoplay"] = audioSource->IsAutoPlay();

	return result;
}