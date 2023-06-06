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

int main(int argc, char* argv[])
{
	// Initialize the factory (register the types)
	Factory::Init();

	// Init window
	Display display("ZEngine 3.0 - Player | By Zac Brannelly", 1366, 768);
	display.Init();

	// Init scripting
	auto scriptSystem = CSharpScriptSystem::GetInstance();
	scriptSystem->Init();

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

	double t = 0.0;
	const double dt = 1.0 / 60.0;

	double currentTime = time->GetTime();
	double accumulator = 0.0;

	while (!display.CloseRequested())
	{
		double newTime = time->GetTime();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			inputManager->Reset();
			display.Update();

			if (mapManager->GetCurrentMap() != nullptr)
			{
				mapManager->GetCurrentMap()->Update();
			}

			time->Tick();

			accumulator -= dt;
		}
		
		mapManager->GetCurrentMap()->Render();
		graphics->Render();

		std::stringstream fps;
		fps << "ZEngine 3.0 - Player | By Zac Brannelly | " << "FPS: " << time->GetFPS();
		display.SetTitle(fps.str());
	}

	assetManager->Shutdown();
	graphics->Shutdown();
	inputManager->Shutdown();
	audioSystem->Shutdown();
	display.Shutdown();
	scriptSystem->Shutdown();
	time->Shutdown();

	return 0;
}