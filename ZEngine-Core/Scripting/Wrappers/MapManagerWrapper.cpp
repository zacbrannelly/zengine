#include "MapManagerWrapper.h"
#include "../../Map/MapManager.h"
#include "../../Map/Map.h"

MapManagerWrapper* MapManagerWrapper::ConstructorImpl(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	return GetInstance();
}

void MapManagerWrapper::InstallImpl(v8::Local<v8::ObjectTemplate> temp)
{
	auto sys = ScriptSystem::GetInstance();

	temp->SetAccessor(sys->GetString("currentMap"), Getter, Setter);
	temp->SetAccessor(sys->GetString("previousMap"), Getter, Setter);
}

void MapManagerWrapper::GetterImpl(std::string name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	auto mapManager = MapManager::GetInstance();

	if (name == "currentMap")
	{
		auto currentMap = mapManager->GetCurrentMap();

		if (currentMap != nullptr)
			info.GetReturnValue().Set(currentMap->GetScriptObject());
		else
			info.GetReturnValue().SetNull();
	}
	else if (name == "previousMap" && mapManager->GetPreviousMap() != nullptr)
	{
		auto prevMap = mapManager->GetPreviousMap();

		if (prevMap != nullptr)
			info.GetReturnValue().Set(prevMap->GetScriptObject());
		else
			info.GetReturnValue().SetNull();
	}
}

void MapManagerWrapper::SetterImpl(std::string name, v8::Local<v8::Value>& value)
{
	auto mapManager = MapManager::GetInstance();

	if (name == "currentMap")
	{
		auto object = v8::Local<v8::Object>::Cast(value);
		auto wrap = v8::Local<v8::External>::Cast(object->GetInternalField(0));
		auto scriptableObject = static_cast<IScriptable*>(wrap->Value());
		auto newMap = static_cast<Map*>(scriptableObject);

		if (newMap != nullptr)
			mapManager->SetCurrentMap(newMap);
	}
}