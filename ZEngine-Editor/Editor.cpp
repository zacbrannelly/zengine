#include "Editor.h"

#include <iostream>
#include <future>

#include <ZEngine-Core/Display/Display.h>
#include <ZEngine-Core/Input/InputManager.h>
#include <ZEngine-Core/Rendering/Graphics.h>
#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Component/MeshRenderer.h>
#include <ZEngine-Core/Rendering/Material.h>
#include <ZEngine-Core/Rendering/Shader.h>
#include <ZEngine-Core/Rendering/Mesh.h>
#include <ZEngine-Core/Component/Transform.h>
#include <ZEngine-Core/Rendering/MeshFactory.h>
#include <ZEngine-Core/Scripting/CSharp/CSharpScriptSystem.h>
#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Assets/Objects/TextureAsset.h>
#include <ZEngine-Core/Rendering/Texture2D.h>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/Objects/ShaderAsset.h>
#include <ZEngine-Core/Assets/Objects/ModelAsset.h>
#include <ZEngine-Core/Assets/Objects/MaterialAsset.h>
#include <ZEngine-Core/Assets/Objects/MapAsset.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Physics/Time.h>
#include <ZEngine-Core/Map/MapManager.h>
#include <ZEngine-Core/Audio/AudioSystem.h>
#include <ZEngine-Core/Utilities/File.h>
#include <ZEngine-Core/GameLoop/GameLoop.h>
#include <glm/glm.hpp>

#include "Project/Project.h"
#include "Exporters/ComponentExporter.h"
#include "UI/GUILibrary.h"
#include "UI/GUIImage.h"
#include "Menus/MainMenuBar.h"
#include "Windows/MapView.h"
#include "Windows/InspectorWindow.h"
#include "Windows/SceneGraphWindow.h"
#include "Windows/GameView.h"
#include "Windows/AssetImporter.h"
#include "Windows/AssetExplorer.h"
#include "Windows/LogWindow.h"
#include "Dialogs/ProjectBrowserDialog.h"
#include "Dialogs/BuildStatusDialog.h"
#include "imgui-includes.h"

Editor::Editor() : _selectedMap(nullptr), _selectedObject(nullptr), _project(nullptr)
{
	Add(new MainMenuBar(this));
	Add(new MapView(this));
	Add(new InspectorWindow(this));
	Add(new SceneGraphWindow(this));
	Add(new GameView(this));
	Add(new AssetImporter());
	Add(new AssetExplorer(this));
	Add(new LogWindow());
	Add(new ProjectBrowserDialog(this));
	Add(new BuildStatusDialog(this));
}

void Editor::Update()
{
	Container::Update();

	// TODO: Add any logic that should be part of the update loop here
}

void Editor::SetProject(Project* project)
{
	const auto assetManager = AssetManager::GetInstance();

	if (_project != nullptr)
	{
		if (_selectedMap != nullptr)
		{
			// TODO: Check if the map is dirty and prompt the user to save
			_selectedMap = nullptr;
		}

		if (_selectedObject != nullptr)
		{
			_selectedObject = nullptr;
		}

		// Save the project
		_project->Save();

		// Unload the project
		assetManager->ReleaseAll();
		assetManager->DeleteAll();

		delete _project;
	}

	_project = project;
	assetManager->SetCatalog(&project->GetCatalog());

	// Build the project in the background
	_buildFuture = _project->BuildAndLoadAsync();
}

Project* Editor::GetProject() const
{
	return _project;
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

	// Init CSharp scripting system
	auto cSharpScriptSystem = CSharpScriptSystem::GetInstance();
	cSharpScriptSystem->Init();

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

	std::function<void()> updateCallback = [&]()
	{
		editorContainer->Update();
	};

	std::function<void()> renderCallback = [&]()
	{
		// Render the GUI
		gui->NewFrame();
		editorContainer->RenderElement();
		gui->EndFrame();

		// Render the grey background
		graphics->Clear(0, 150, 150, 150, 255);
		graphics->Viewport(0, 0, 0, display.GetWidth(), display.GetHeight());
		graphics->Touch(0);

		// Head to the next frame (no draw calls beyond this point)
		graphics->Render();
	};

	GameLoop gameLoop(&display, 1 / 60.0, updateCallback, renderCallback);
	gameLoop.StartLoop();

	// Clean up the GUI
	delete editorContainer;

	// Clean up
	gui->Shutdown();
	assetManager->Shutdown();
	graphics->Shutdown();
	audioSystem->Shutdown();
	inputManager->Shutdown();
	display.Shutdown();
	cSharpScriptSystem->Shutdown();
	time->Shutdown();

	ComponentExporter::Cleanup();

	return 0;
}