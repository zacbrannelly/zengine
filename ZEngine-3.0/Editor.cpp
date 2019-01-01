#include "Editor.h"

#include <iostream>

#include <ZEngine-Core\Display\Display.h>
#include <ZEngine-Core\Input\InputManager.h>
#include <ZEngine-Core\Rendering\Graphics.h>
#include <ZEngine-Core\Misc\Factory.h>
#include <ZEngine-Core\Map\Map.h>
#include <ZEngine-Core\Map\Objects\Entity.h>
#include <ZEngine-Core\Component\MeshRenderer.h>
#include <ZEngine-Core\Rendering\Material.h>
#include <ZEngine-Core\Rendering\Shader.h>
#include <ZEngine-Core\Rendering\Mesh.h>
#include <ZEngine-Core\Component\Transform.h>
#include <ZEngine-Core\Rendering\MeshFactory.h>
#include <ZEngine-Core\Scripting\ScriptSystem.h>
#include <ZEngine-Core\Scripting\Script.h>
#include <ZEngine-Core\Component\ScriptComponent.h>
#include <ZEngine-Core\Component\Camera.h>
#include <ZEngine-Core\Assets\Objects\TextureAsset.h>
#include <ZEngine-Core\Rendering\Texture2D.h>
#include <ZEngine-Core\Assets\AssetManager.h>
#include <ZEngine-Core\Assets\Objects\ShaderAsset.h>
#include <ZEngine-Core\Assets\Objects\ModelAsset.h>
#include <ZEngine-Core\Assets\Objects\MaterialAsset.h>
#include <ZEngine-Core\Assets\Objects\MapAsset.h>
#include <ZEngine-Core\Assets\AssetCatalog.h>
#include <ZEngine-Core\Physics\Time.h>
#include <ZEngine-Core\Map\MapManager.h>
#include <ZEngine-Core\Audio\AudioSystem.h>
#include <glm/glm.hpp>

#include "ComponentExporter.h"
#include "GUILibrary.h"
#include "MainMenuBar.h"
#include "MapView.h"
#include "InspectorWindow.h"
#include "SceneGraphWindow.h"
#include "GUIImage.h"
#include "GameView.h"
#include "AssetImporter.h"
#include "AssetExplorer.h"
#include "LogWindow.h"

#include "imgui-includes.h"

Editor::Editor()
{
	// Load a test catalog (replace this with a way for the user to load the catalog)
	auto testCatalog = new AssetCatalog();
	testCatalog->LoadCatalog();
	AssetManager::GetInstance()->SetCatalog(testCatalog);

	Add(new MainMenuBar());
	Add(new MapView(this));
	Add(new InspectorWindow(this));
	Add(new SceneGraphWindow(this));
	Add(new GameView(this));
	Add(new AssetImporter());
	Add(new AssetExplorer(this));
	Add(new LogWindow());
}

void Editor::Update()
{

}

void Editor::SetSelectedMap(Map* map)
{
	_selectedMap = map;

	MapManager::GetInstance()->SetCurrentMap(map);
}

Map* Editor::GetSelectedMap() const
{
	return _selectedMap;
}

void Editor::SetSelectedEntity(Entity* entity)
{
	_selectedObject = entity;
}

Entity* Editor::GetSelectedEntity() const
{
	return _selectedObject;
}

Editor::~Editor()
{
}

int main(int argc, char* argv[])
{
	// Initialize the factory (register the types)
	Factory::Init();

	//TODO: Register any editor specific ZObject's here
	ComponentExporter::RegisterAllTypes();

	// Init scripting system
	auto scriptSystem = ScriptSystem::GetInstance();
	scriptSystem->Init(argv[0]);

	// Init window
	Display display("ZEngine 3.0 | By Zac Brannelly", 1920, 1060);
	display.Init();

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

	// Init GUI sub-system
	auto gui = GUILibrary::GetInstance();
	gui->Init(&display);

	auto time = Time::GetInstance();
	time->Init();

	// This container will hold all of the GUI elements
	Editor* editorContainer = new Editor();

	while (!display.CloseRequested())
	{
		// Reset the input manager (so the release function works)
		inputManager->Reset();

		// Poll for input 
		display.Update();

		// Render the GUI
		gui->NewFrame();

		editorContainer->Update();
		editorContainer->RenderElement();

		gui->EndFrame();

		// Render the grey background
		graphics->Clear(0, 150, 150, 150, 255);
		graphics->Viewport(0, 0, 0, display.GetWidth(), display.GetHeight());
		graphics->Touch(0);

		// Head to the next frame (no draw calls beyond this point)
		graphics->Render();

		time->Tick();
	}

	// Clean up the GUI
	delete editorContainer;

	// Clean up
	gui->Shutdown();
	assetManager->Shutdown();
	graphics->Shutdown();
	audioSystem->Shutdown();
	inputManager->Shutdown();
	display.Shutdown();
	scriptSystem->Shutdown();
	time->Shutdown();

	ComponentExporter::Cleanup();

	return 0;
}