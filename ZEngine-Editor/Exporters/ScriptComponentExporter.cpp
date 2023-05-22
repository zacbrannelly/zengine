#include "ScriptComponentExporter.h"
#include <ZEngine-Core/Component/ScriptComponent.h>
#include <ZEngine-Core/Scripting/Script.h>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Assets/Objects/ScriptAsset.h>

using namespace nlohmann;

json ScriptComponentExporter::ExportImpl(ZObject* obj)
{
	auto scriptComp = static_cast<ScriptComponent*>(obj);
	auto assetManager = AssetManager::GetInstance();
	auto catalog = assetManager->GetCatalog();

	json result;

	result["name"] = scriptComp->GetName();
	result["type"] = scriptComp->GetType();

	int scriptId = -1;
	
	// Find the script's asset obj 
	// TODO: this needs to be refactored some how for the better)
	for (const auto& asset : assetManager->GetAssets())
	{
		if (asset->GetType() == SCRIPT_ASSET)
		{
			auto scriptAsset = asset->Cast<ScriptAsset>();

			if (scriptAsset->GetScript() == scriptComp->GetScript() && catalog != nullptr)
			{
				scriptId = catalog->GetAssetIDFromPath(scriptAsset->GetPath());
				break;
			}
		}
	}

	if (scriptId < 0)
		return json();

	result["script"] = scriptId;

	return result;
}