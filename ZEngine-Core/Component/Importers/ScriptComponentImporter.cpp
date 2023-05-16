#include "ScriptComponentImporter.h"
#include "../../Misc/Factory.h"
#include "../ScriptComponent.h"
#include "../../Scripting/Script.h"
#include "../../Assets/AssetManager.h"
#include "../../Assets/AssetCatalog.h"
#include "../../Assets/Objects/ScriptAsset.h"

using namespace std;
using namespace nlohmann;

ZObject* ScriptComponentImporter::ImportImpl(string name, json& values)
{
	auto scriptComp = Factory::CreateInstance<ScriptComponent>(name, SCRIPT_COMPONENT);
	auto assetManager = AssetManager::GetInstance();

	if (HasKey("script", values))
	{
		int scriptId = ReadInt("script", values);

		string path;
		ObjectType type;
		if (assetManager->GetCatalog()->GetAssetPathFromID(scriptId, path, type))
		{
			auto scriptAsset = assetManager->FindAssetFromPath(path);

			if (scriptAsset == nullptr)
				scriptAsset = assetManager->LoadAsset(name, path, type);

			if (scriptAsset != nullptr)
			{
				scriptComp->SetScript(scriptAsset->Cast<ScriptAsset>()->GetScript());
			}
		}
	}

	return scriptComp;
}