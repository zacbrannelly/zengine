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
#include <ZEngine-Core\Scripting\ScriptSystem.h>
#include <ZEngine-Core\Scripting\Script.h>
#include <ZEngine-Core\Component\ScriptComponent.h>
#include <ZEngine-Core\Component\Camera.h>
#include <ZEngine-Core\Assets\Objects\TextureAsset.h>
#include <ZEngine-Core\Rendering\Texture2D.h>
#include <ZEngine-Core\Assets\AssetManager.h>
#include <ZEngine-Core\Assets\Objects\ShaderAsset.h>
#include <glm/glm.hpp>

#include "GUILibrary.h"
#include "MainMenuBar.h"
#include "MapView.h"
#include "InspectorWindow.h"
#include "SceneGraphWindow.h"
#include "GUIImage.h"

#include "imgui-includes.h"

Editor::Editor()
{
	auto testMap = Factory::CreateInstance<Map>("test_map", ObjectType::MAP);
	SetSelectedMap(testMap);

	AssetManager::GetInstance()->LoadAsset("standard_unlit", "shaders/standard_unlit.shader", ObjectType::SHADER_ASSET);
	AssetManager::GetInstance()->LoadAsset("pbr_direct", "shaders/pbr_direct.shader", ObjectType::SHADER_ASSET);

	// Add camera object to map
	{
		auto cameraObject = Factory::CreateInstance<Entity>("Main Camera", ObjectType::ENTITY);
		cameraObject->AddComponent(Factory::CreateInstance<Camera>("Camera", ObjectType::CAMERA));
		testMap->Add(cameraObject);
	}

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

		auto testObject2 = Factory::CreateInstance<Entity>("test_object2", ObjectType::ENTITY);

		// Create mesh renderer with sphere mesh attached
		mesh = MeshFactory::CreateCube("Rect");
		//mesh->SetColors(std::vector<glm::vec4>(mesh->GetVertices().size(), { 1, 0, 0, 1 }));
		meshRenderer = Factory::CreateInstance<MeshRenderer>("Mesh Renderer", ObjectType::MESH_RENDERER);

		// Example of loading a texture into a material (TODO: Fix memory leak and finish texture class properly)
		{
			auto texturedMaterial = Factory::CreateInstance<Material>("textured mat", ObjectType::MATERIAL);
			texturedMaterial->SetShader(AssetManager::GetInstance()->GetAsset<ShaderAsset>("pbr_direct")->GetShader());

			// Material light properties
			texturedMaterial->RegisterSampler("albedoTexture");
			texturedMaterial->RegisterUniform("roughness", bgfx::UniformType::Vec4, 1);
			texturedMaterial->RegisterUniform("metallic", bgfx::UniformType::Vec4, 1);
			texturedMaterial->RegisterUniform("ao", bgfx::UniformType::Vec4, 1);

			// Light positions and colours
			texturedMaterial->RegisterUniform("lightPositions", bgfx::UniformType::Vec4, 4);
			texturedMaterial->RegisterUniform("lightColors", bgfx::UniformType::Vec4, 4);

			// World position of the camera
			texturedMaterial->RegisterUniform("camPos", bgfx::UniformType::Vec4, 1);

			auto assetManager = AssetManager::GetInstance();
			auto textureAsset = assetManager->LoadAsset("test texture", "test.png", ObjectType::TEXTURE_ASSET)->Cast<TextureAsset>();
			textureAsset->LoadTexture();

			texturedMaterial->SetTexture("albedoTexture", textureAsset->GetTexture()->GetHandle());
			texturedMaterial->SetUniform("roughness", new glm::vec4(0.5f), 1);
			texturedMaterial->SetUniform("metallic", new glm::vec4(0.5f), 1);
			texturedMaterial->SetUniform("ao", new glm::vec4(1.0f), 1);

			auto lightPos = new std::vector<glm::vec4>
			{
				{ -10, -10, -10, 1 },
				{  10,  10, -10, 1 },
				{ -10,  10, -10, 1 },
				{  10, -10, -10, 1 }
			};

			auto lightColors = new std::vector<glm::vec4>
			{
				{ 600, 600, 600, 0 },
				{ 600, 600, 600, 0 },
				{ 600, 600, 600, 0 },
				{ 600, 600, 600, 0 },
			};

			texturedMaterial->SetUniform("lightPositions", &(*lightPos)[0], lightPos->size());
			texturedMaterial->SetUniform("lightColors", &(*lightColors)[0], lightColors->size());
			texturedMaterial->SetUniform("camPos", new glm::vec4(0, 0, -10, 1), 1);

			meshRenderer->SetMaterial(texturedMaterial);
		}
		
		meshRenderer->SetMesh(mesh);
		testObject2->AddComponent(meshRenderer);

		// Create a test scripting component and add to test_object2
		{
			auto script = Factory::CreateInstance<Script>("TestComponent", ObjectType::SCRIPT);

			if (script->CompileFromFile("scripts/test.js"))
			{
				script->Execute();

				auto scriptComp = Factory::CreateInstance<ScriptComponent>("TestComponent", ObjectType::SCRIPT_COMPONENT);
				scriptComp->SetScript(script);

				testObject2->AddComponent(scriptComp);
			}
		}

		testObject2->GetTransform()->SetParent(testObject->GetTransform());
		testMap->Add(testObject2);

		SetSelectedEntity(testObject);
	}

	Add(new MainMenuBar());
	Add(new MapView(this));
	Add(new InspectorWindow(this));
	Add(new SceneGraphWindow(this));
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

	//TODO: Register any editor specific ZObject's here

	// Init scripting system
	auto scriptSystem = ScriptSystem::GetInstance();
	scriptSystem->Init(argv[0]);

	// Init window
	Display display("ZEngine 3.0 | By Zac Brannelly", 1920, 1060);
	display.Init();

	// Init graphics sub-system
	auto graphics = Graphics::GetInstance();
	graphics->Init(&display);

	// Init asset management system
	auto assetManager = AssetManager::GetInstance();
	assetManager->Init();

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
	assetManager->Shutdown();
	graphics->Shutdown();
	display.Shutdown();
	scriptSystem->Shutdown();

	return 0;
}