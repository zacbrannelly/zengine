#include <iostream>
#include <sstream>

#include "../ZEngine-Core/Display/Display.h"
#include "../ZEngine-Core/Misc/Factory.h"
#include "../ZEngine-Core/Input/InputManager.h"
#include "../ZEngine-Core/Rendering/Graphics.h"
#include "../ZEngine-Core/Assets/AssetManager.h"
#include "../ZEngine-Core/Assets/AssetCatalog.h"
#include "../ZEngine-Core/Assets/Objects/MapAsset.h"
#include "../ZEngine-Core/Physics/Time.h"
#include "../ZEngine-Core/Map/Map.h"
#include "../ZEngine-Core/Map/MapManager.h"
#include "../ZEngine-Core/Audio/AudioSystem.h"
#include "../ZEngine-Core/Scripting/CSharp/CSharpScriptSystem.h"
#include "../ZEngine-Core/GameLoop/GameLoop.h"

using namespace ZEngine;

int main(int argc, char* argv[])
{
	// Initialize the factory (register the types)
	Factory::Init();

	// Init window
	Display display("ZEngine - Player", 1366, 768);
	display.Init();

// TODO: Support scripting on iOS.
#if !TARGET_OS_IPHONE
	// Init scripting
	auto scriptSystem = CSharpScriptSystem::GetInstance();
	scriptSystem->Init();
#endif

	auto inputManager = InputManager::GetInstance();
	inputManager->Init(&display);

	auto audioSystem = AudioSystem::GetInstance();
	audioSystem->Init();

	// Init graphics sub-system
	auto graphics = Graphics::GetInstance();
	graphics->Init(&display);

	// Init asset management system
	auto assetManager = AssetManager::GetInstance();
	assetManager->Init();

	// Init map management system
	auto mapManager = MapManager::GetInstance();

	// Init Time management
	auto time = Time::GetInstance();
	time->Init();

	auto catalog = new AssetCatalog();
	catalog->LoadCatalog("catalog.zengine");

	assetManager->SetCatalog(catalog);

	auto mapAsset = assetManager->LoadAsset("test map", "Assets/maps/test_map.map", MAP_ASSET)->Cast<MapAsset>();

	if (mapAsset == nullptr)
	{
		std::cout << "Failed to load the map!" << std::endl;
		return -1;
	}

	mapManager->SetCurrentMap(mapAsset->GetMap());

	std::function<void()> updateCallback = [&]()
	{
		if (mapManager->GetCurrentMap() != nullptr)
		{
			mapManager->GetCurrentMap()->Update();
		}
	};

	std::function<void()> renderCallback = [&]()
	{
		if (mapManager->GetCurrentMap() != nullptr)
		{
			mapManager->GetCurrentMap()->Render();
		}

		graphics->Render();

		std::stringstream fps;
		fps << "ZEngine - Player (" << "FPS: " << time->GetFPS() << ")";
		display.SetTitle(fps.str());
	};

	GameLoop gameLoop(&display, 1.0 / 60.0, updateCallback, renderCallback);
	gameLoop.StartLoop();

	assetManager->Shutdown();
	graphics->Shutdown();
	inputManager->Shutdown();
	audioSystem->Shutdown();
	display.Shutdown();

// TODO: Support scripting on iOS.
#if !TARGET_OS_IPHONE
	scriptSystem->Shutdown();
#endif

	time->Shutdown();

	return 0;
}