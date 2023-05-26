#include "CSharpScriptComponentImporter.h"
#include "../../Misc/Factory.h"
#include "../CSharpScriptComponent.h"
#include "../../Scripting/CSharp/CSharpScript.h"
#include "../../Assets/AssetManager.h"
#include "../../Assets/AssetCatalog.h"
#include "../../Assets/Objects/CSharpScriptAsset.h"

using namespace std;
using namespace nlohmann;

ZObject* CSharpScriptComponentImporter::ImportImpl(string name, json& values)
{
	auto scriptComp = Factory::CreateInstance<CSharpScriptComponent>(name, CSHARP_SCRIPT_COMPONENT);
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
				scriptComp->SetScript(scriptAsset->Cast<CSharpScriptAsset>()->GetScript());
			}
		}
	}

	return scriptComp;
}
