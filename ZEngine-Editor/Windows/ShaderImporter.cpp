#include "ShaderImporter.h"
#include "../Exporters/ShaderBuilder.h"
#include <ZEngine-Core/Utilities/File.h>
#include <ZEngine-Core/Utilities/Directory.h>

#include <string>
#include <iostream>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <nlohmann/json.hpp>

ShaderImporter::ShaderImporter(Container* owner) : ShaderImporter(owner, "\0")
{
	
}

ShaderImporter::ShaderImporter(Container* owner, std::string path) : GUIWindow("Shader Importer", 400, 500, false)
{
	_owner = owner;
	_pathTypeKnown = path == "\0";
	_unknownFile = path;

	_nameTextField = new GUITextField("Name");
	Add(_nameTextField);

	SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
}

bool ShaderImporter::Validate()
{
	for (auto& pass : _passes)
	{
		// Copy from the buffers into the actual string values
		pass.vertShaderPath = std::string(pass.vertShaderPathBuffer);
		pass.fragShaderPath = std::string(pass.fragShaderPathBuffer);
		pass.varyingDefPath = std::string(pass.varyingDefPathBuffer);

		File vertFile(pass.vertShaderPath);
		File fragFile(pass.fragShaderPath);
		File varyFile(pass.varyingDefPath);

		if (!vertFile.Exists())
		{
			std::cout << "SHADER_IMPORTER: Failed to locate vertex shader code at: " << vertFile.GetPath() << std::endl;
			return false;
		}

		if (!fragFile.Exists())
		{
			std::cout << "SHADER_IMPORTER: Failed to locate fragment shader code at: " << fragFile.GetPath() << std::endl;
			return false;
		}

		if (!varyFile.Exists())
		{
			std::cout << "SHADER_IMPORTER: Failed to locate varying def code at: " << varyFile.GetPath() << std::endl;
			return false;
		}
	}

	return true;
}

bool ShaderImporter::Create()
{
	if (Validate())
	{
		using namespace nlohmann;

		json root;
		auto name = root["name"] = _nameTextField->GetText();
		auto& passes = root["pass"] = json::array_t();

		// Create the binaries for all the passes & create JSON objects
		for (const auto& pass : _passes)
		{
			if (!ShaderBuilder::BuildToFile(SB_VERTEX, pass.vertShaderPath, pass.varyingDefPath))
			{
				std::cout << "SHADER_IMPORTER: Failed to build vertex shader code at path: " << pass.vertShaderPath << std::endl;
				return false;
			}

			if (!ShaderBuilder::BuildToFile(SB_FRAGMENT, pass.fragShaderPath, pass.varyingDefPath))
			{
				std::cout << "SHADER_IMPORTER: Failed to build fragment shader code at path: " << pass.vertShaderPath << std::endl;
				return false;
			}

			json::object_t passObj;
			passObj["vertex"] = pass.vertShaderPath + ".bin";
			passObj["fragment"] = pass.fragShaderPath + ".bin";

			passes.push_back(passObj);
		}


		// Write JSON objects to shader asset file
		Directory assetsFolder("Assets/");

		if (!assetsFolder.Exists())
		{
			if (!assetsFolder.Create())
				return false;
		}

		Directory shadersFolder("Assets/shaders/");

		if (!shadersFolder.Exists())
		{
			if (!shadersFolder.Create())
				return false;
		}

		File assetFile("Assets/shaders/" + name.get<std::string>() + ".shader");

		auto data = root.dump(4);
		auto result = assetFile.WriteContent(data.c_str(), data.size());

		auto catalog = AssetManager::GetInstance()->GetCatalog();

		if (catalog != nullptr && !catalog->HasAsset(assetFile.GetPath()))
		{
			catalog->RegisterAsset(assetFile.GetPath(), SHADER_ASSET);
			catalog->SaveCatalog(catalog->GetLastCatalogPath());
		}
		else if (catalog == nullptr)
		{
			// TODO: Show error message asking to create a catalog.
		}
	
		return result;
	}

	return false;
}

void ShaderImporter::ProcessInput() 
{

}

void ShaderImporter::RenderInWindow()
{
	if (!_pathTypeKnown)
	{
		std::string type = "Unspecified";
		if (ImGui::BeginCombo("File Shader Type", type.c_str()))
		{
			if (ImGui::Selectable("Vertex"))
			{
				ShaderImporterPass pass;
				memcpy(pass.vertShaderPathBuffer, &_unknownFile[0], _unknownFile.size());

				_passes.push_back(pass);
				_pathTypeKnown = true;
			}
			
			if (ImGui::Selectable("Fragment"))
			{
				ShaderImporterPass pass;
				memcpy(pass.fragShaderPathBuffer, &_unknownFile[0], _unknownFile.size());

				_passes.push_back(pass);
				_pathTypeKnown = true;
			}

			if (ImGui::Selectable("Varying Definition"))
			{
				ShaderImporterPass pass;
				memcpy(pass.varyingDefPathBuffer, &_unknownFile[0], _unknownFile.size());

				_passes.push_back(pass);
				_pathTypeKnown = true;
			}

			ImGui::EndCombo();
		}

		return;
	}

	auto it = _passes.begin();
	for (int i = 0; i < _passes.size(); i++, it++)
	{
		auto& pass = *it;

		if (ImGui::CollapsingHeader((std::string("Pass: ") + std::to_string(i)).c_str()))
		{
			ImGui::InputText("Vertex Shader Path", pass.vertShaderPathBuffer, SHADER_IMPORTER_BUFFER_SIZE);
			ImGui::InputText("Fragment Shader Path", pass.fragShaderPathBuffer, SHADER_IMPORTER_BUFFER_SIZE);
			ImGui::InputText("Varying Definitions Path", pass.varyingDefPathBuffer, SHADER_IMPORTER_BUFFER_SIZE);

			ImGui::NextLine();

			if (ImGui::Button((std::string("Remove Pass ###") + std::to_string(i)).c_str()))
			{
				_passes.erase(it);
				break;
			}
		}
	}

	ImGui::Separator();

	if (ImGui::Button("Add Pass.."))
	{
		_passes.push_back(ShaderImporterPass());
	}

	ImGui::Separator();

	if (ImGui::Button("Create"))
	{
		if (Create())
			SetCloseRequested(true);
	}

	ImGui::SameLine();
	if (ImGui::Button("Cancel"))
	{
		SetCloseRequested(true);
	}
}

void ShaderImporter::RenderElement()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(450, 50), ImVec2(500, 800));
	GUIWindow::RenderElement();
}

ShaderImporter::~ShaderImporter()
{
	for (auto& pass : _passes)
	{
		delete[] pass.vertShaderPathBuffer;
		delete[] pass.fragShaderPathBuffer;
		delete[] pass.varyingDefPathBuffer;
	}
}