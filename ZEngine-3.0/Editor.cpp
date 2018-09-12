#include "Editor.h"

#include <iostream>

#include <ZEngine-Core\Display\Display.h>
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
#include <glm/glm.hpp>

#include "GUILibrary.h"
#include "MainMenuBar.h"
#include "MapView.h"
#include "InspectorWindow.h"
#include "GUIImage.h"

Editor::Editor()
{
	auto testMap = Factory::CreateInstance<Map>("test_map", ObjectType::MAP);
	SetSelectedMap(testMap);

	// Add object to the test map
	{
		auto testObject = Factory::CreateInstance<Entity>("test_object", ObjectType::ENTITY);

		// Create material for mesh to be created
		auto shader = Factory::CreateInstance<Shader>("cubes", ObjectType::SHADER);
		shader->Load("vs_cubes.bin", "fs_cubes.bin");
		auto material = Factory::CreateInstance<Material>("test material", ObjectType::MATERIAL);
		material->SetShader(shader);

		// Create mesh renderer with sphere mesh attached
		auto mesh = MeshFactory::CreateSphere("Sphere");
		auto meshRenderer = Factory::CreateInstance<MeshRenderer>("Mesh Renderer", ObjectType::MESH_RENDERER);
		meshRenderer->SetMesh(mesh);
		meshRenderer->SetMaterial(material);
		testObject->AddComponent(meshRenderer);

		testMap->Add(testObject);

		SetSelectedEntity(testObject);
	}

	Add(new MainMenuBar());
	Add(new MapView(testMap));
	Add(new InspectorWindow(this));

}

void Editor::Update()
{
}

void Editor::SetSelectedMap(Map* map)
{
	_selectedMap = map;
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

	// TODO: Register any editor specific ZObject's here

	// Init window
	Display display("ZEngine 3.0 | By Zac Brannelly", 1920, 1060);
	display.Init();

	// Init graphics sub-system
	auto graphics = Graphics::GetInstance();
	graphics->Init(&display);

	// Init GUI sub-system
	auto gui = GUILibrary::GetInstance();
	gui->Init(&display);

	// This container will hold all of the GUI elements
	Editor* editorContainer = new Editor();

	while (!display.CloseRequested())
	{
		display.Update();

		editorContainer->Update();

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
	}

	// Clean up the GUI
	delete editorContainer;

	// Clean up
	gui->Shutdown();
	graphics->Shutdown();
	display.Shutdown();

	return 0;
}