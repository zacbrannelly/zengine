#include "MaterialEditor.h"
#include "../Builders/MaterialBuilder.h"
#include <ZEngine-Core/Utilities/File.h>
#include "../UI/GUITextField.h"
#include "../UI/GUIImage.h"
#include "../Dialogs/UnsavedDialog.h"

#include <ZEngine-Core/Input/InputManager.h>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Assets/Objects/ShaderAsset.h>
#include <ZEngine-Core/Assets/Objects/MaterialAsset.h>
#include <ZEngine-Core/Assets/Objects/TextureAsset.h>
#include <ZEngine-Core/Rendering/Material.h>
#include <ZEngine-Core/Rendering/Shader.h>

MaterialEditor::MaterialEditor(std::string assetPath) : GUIWindow("Material Editor - " + assetPath, 500, 400, false)
{
	_assetFile = new File(assetPath);
	_unsavedDialog = new UnsavedDialog();
	_showUnsavedDialog = false;

	if (!LoadData())
		SetCloseRequested(true);
}

bool MaterialEditor::LoadData()
{
	auto assetManager = AssetManager::GetInstance();
	auto catalog = assetManager->GetCatalog();

	MaterialAsset* tempAsset = nullptr;
	Material* material = nullptr;

	auto asset = assetManager->FindAssetFromPath(_assetFile->GetPath());
	
	if (asset == nullptr)
	{
		tempAsset = new MaterialAsset(_assetFile->GetFilename());

		if (!tempAsset->Load(_assetFile->GetPath()))
		{
			return false;
		}

		material = tempAsset->GetMaterial();
	}
	else
		material = asset->Cast<MaterialAsset>()->GetMaterial();

	_data.name = material->GetName();
	_data.shaderID = uuids::uuid();
	_data.shaderPath = "";

	// Locate the ID of the shader
	for (auto& asset : assetManager->GetAssets())
	{
		if (asset->GetType() == SHADER_ASSET)
		{
			auto shaderAsset = asset->Cast<ShaderAsset>();
			
			if (shaderAsset->GetShader() == material->GetShader())
			{
				_data.shaderID = catalog->GetAssetIDFromPath(shaderAsset->GetPath());
				_data.shaderPath = shaderAsset->GetPath();
				break;
			}
		}
	}

	if (_data.shaderPath == "")
	{
		if (tempAsset != nullptr)
			delete tempAsset;

		return false;
	}

	// Copy all of the uniforms from the material into our data structure
	for (const auto& uniformPair : material->GetUniforms())
	{
		const auto& source = uniformPair.second;
		Uniform copy; 
		
		copy.numElements = uniformPair.second.numElements;
		copy.type = uniformPair.second.type;

		switch (copy.type)
		{
		case bgfx::UniformType::Vec4:
			copy.data = new glm::vec4[copy.numElements];
			memcpy(copy.data, source.data, copy.numElements * sizeof(glm::vec4));
			break;
		case bgfx::UniformType::Mat3:
			copy.data = new glm::mat3[copy.numElements];
			memcpy(copy.data, source.data, copy.numElements * sizeof(glm::mat3));
			break;
		case bgfx::UniformType::Mat4:
			copy.data = new glm::mat4[copy.numElements];
			memcpy(copy.data, source.data, copy.numElements * sizeof(glm::mat4));
			break;
		}

		_data.uniforms[uniformPair.first] = copy;

		auto nameField = new GUITextField("Name###nametf" + uniformPair.first);
		nameField->SetText(uniformPair.first);
		_data.uniformNameFields[uniformPair.first] = nameField;
	}

	// Copy all samplers into our data structure
	for (const auto& samplerPair : material->GetSamplers())
	{
		const auto& source = samplerPair.second;
		MaterialEditorSampler sampler; 

		for (auto& asset : assetManager->GetAssets())
		{
			if (asset->GetType() == TEXTURE_ASSET)
			{
				auto textureAsset = asset->Cast<TextureAsset>();

				if (textureAsset->GetTexture()->GetHandle().idx == source.texture.idx)
				{
					sampler.assetID = catalog->GetAssetIDFromPath(textureAsset->GetPath());
					sampler.assetPath = asset->GetPath();
					break;
				}
			}
		}
		
		sampler.sampler.texture = source.texture;
		sampler.previewImage = new GUIImage(sampler.sampler.texture, 50, 50);

		sampler.nameField = new GUITextField("Name###nametf" + samplerPair.first);
		sampler.nameField->SetText(samplerPair.first);

		_data.samplers[samplerPair.first] = sampler;
	}

	if (tempAsset != nullptr)
		delete tempAsset;

	return true;
}

bool MaterialEditor::Save()
{
	if (MaterialBuilder::BuildToFile(_assetFile->GetPath(), _data))
	{
		HotReload();
		SetDirty(false);
		return true;
	}

	return false;
}

void MaterialEditor::HotReload()
{
	auto asset = AssetManager::GetInstance()->FindAssetFromPath(_assetFile->GetPath());

	if (asset != nullptr)
	{
		asset->Load(_assetFile->GetPath());
	}
}

void MaterialEditor::ProcessInput()
{
	if (ImGui::IsWindowFocused())
	{
		auto input = InputManager::GetInstance();
		if (input->GetButtonPressed(BUTTON_KEY_S) && input->ButtonHasModifier(BUTTON_KEY_S, BUTTON_MOD_CONTROL))
		{
			Save();
		}
	}
}

bool MaterialEditor::AllowClose()
{
	if (IsDirty())
		_showUnsavedDialog = true;

	return !IsDirty();
}

void MaterialEditor::RenderInWindow()
{
	if (IsCloseRequested())
		return;

	auto assetManager = AssetManager::GetInstance();
	auto catalog = assetManager->GetCatalog();

	if (ImGui::BeginCombo("Shader", _data.shaderPath.c_str()))
	{
		auto availShaders = catalog->GetAssetsByType(SHADER_ASSET);

		for (auto& shader : availShaders)
		{
			if (ImGui::Selectable(shader.path.c_str()))
			{
				_data.shaderID = shader.id;
				_data.shaderPath = shader.path.c_str();

				SetDirty(true);
			}
		}

		ImGui::EndCombo();
	}

	if (ImGui::CollapsingHeader("Uniforms"))
	{
		if (ImGui::Button("Add Uniform.."))
		{
			if (_data.uniforms.find("New_Uniform") != _data.uniforms.end())
				return;

			// Ensure the previous label doesn't contain New_Uniform
			for (auto& pair : _data.uniformNameFields)
			{
				pair.second->SetLabel("Name###nametf" + pair.first);
			}

			Uniform newUniform;
			newUniform.data = new float[4];
			memset(newUniform.data, 0, 4 * sizeof(float));

			newUniform.numElements = 1;
			newUniform.type = bgfx::UniformType::Vec4;
		
			_data.uniforms["New_Uniform"] = newUniform;

			auto nameField = new GUITextField("Name###nametfNew_Uniform");
			nameField->SetText("New_Uniform");
			_data.uniformNameFields["New_Uniform"] = nameField;
		}

		ImGui::Separator();

		std::map<std::string, std::string> rename;

		for (auto& pair : _data.uniforms)
		{
			// Render the name text field
			auto nameField = _data.uniformNameFields[pair.first];
			nameField->RenderElement();

			// Request the uniform to be renamed if it has been changed
			if (nameField->GetText() != pair.first)
			{
				rename[pair.first] = nameField->GetText();
				SetDirty(true);
			}

			int numElements = pair.second.numElements;
			if (ImGui::InputInt((std::string("Number of elements###N0E") + pair.first).c_str(), &numElements))
			{
				SetDirty(true);

				if (numElements == 0)
					return;

				int num = 4;
				switch (pair.second.type)
				{
				case bgfx::UniformType::Mat3:
					num = 9;
					break;
				case bgfx::UniformType::Mat4:
					num = 16;
					break;
				}

				// Resize the memory for the new number of elements
				auto newData = new float[num * numElements];

				memset(newData, 0, sizeof(float) * num * numElements);
				memcpy(newData, pair.second.data, sizeof(float) * num * (numElements < pair.second.numElements ? numElements : pair.second.numElements));

				pair.second.data = newData;
				pair.second.numElements = numElements;
			}
			
			// Only allow vectors to be edited (maybe do matrices later)
			if (pair.second.type == bgfx::UniformType::Vec4)
			{
				// Draw a widget for each element
				for (int i = 0; i < pair.second.numElements; i++)
				{
					auto stringId = std::to_string(i);
					if (ImGui::DragFloat4((std::string("Value " + stringId + "###") + pair.first + stringId).c_str(), (float*)(pair.second.data) + i * 4, 0.01f))
					{
						SetDirty(true);
					}
				}
			}

			if (ImGui::Button((std::string("Remove###rmvbutton") + pair.first).c_str()))
			{
				delete _data.uniformNameFields[pair.first];
				delete _data.uniforms[pair.first].data;

				_data.uniformNameFields.erase(pair.first);
				_data.uniforms.erase(pair.first);
				SetDirty(true);

				break;
			}

			ImGui::Separator();
		}

		// Rename the fields that were requested to be renamed
		for (const auto& pair : rename)
		{
			// Name is already taken so skip
			if (_data.uniforms.find(pair.second) != _data.uniforms.end())
				continue;

			_data.uniforms[pair.second] = _data.uniforms[pair.first];
			_data.uniforms.erase(pair.first);

			_data.uniformNameFields[pair.second] = _data.uniformNameFields[pair.first];
			_data.uniformNameFields.erase(pair.first);
		}
	}

	if (ImGui::CollapsingHeader("Textures"))
	{
		if (ImGui::Button("Add Texture.."))
		{
			if (_data.samplers.find("New_Texture") != _data.samplers.end())
				return;

			// Ensure labels are correct
			for (auto& pair : _data.samplers)
				pair.second.nameField->SetLabel("Name###nametf" + pair.first);

			MaterialEditorSampler newSampler;

			newSampler.assetPath = "None";
			newSampler.nameField = new GUITextField("Name###nametfNew_Texture");
			newSampler.nameField->SetText("New_Texture");
			newSampler.assetID = uuids::uuid();
			newSampler.previewImage = nullptr;
			newSampler.sampler.texture = { bgfx::kInvalidHandle };

			_data.samplers["New_Texture"] = newSampler;
		}

		ImGui::Separator();

		std::map<std::string, std::string> rename;

		for (auto& pair : _data.samplers)
		{
			// Render the name text field
			auto nameField = pair.second.nameField;
			nameField->RenderElement();
			
			// Request a rename
			if (nameField->GetText() != pair.first)
			{
				SetDirty(true);
				rename[pair.first] = nameField->GetText();
			}

			auto textureName = pair.second.assetPath;
			if (ImGui::BeginCombo(("Texture###2" + pair.first).c_str(), textureName.c_str()))
			{
				auto availTextures = catalog->GetAssetsByType(TEXTURE_ASSET);

				for (auto texture : availTextures)
				{
					if (ImGui::Selectable(texture.path.c_str()) && texture.path != pair.second.assetPath)
					{
						pair.second.assetID = catalog->GetAssetIDFromPath(texture.path);
						pair.second.assetPath = texture.path;

						// Find/Load the new texture (so we can preview it)
						auto newTexture = assetManager->FindAssetFromPath(texture.path);

						if (newTexture == nullptr)
						{
							newTexture = assetManager->LoadAsset(texture.path, texture.path, TEXTURE_ASSET);
							newTexture->Cast<TextureAsset>()->LoadTexture();
						}

						// Update texture handle (for preview image)
						pair.second.sampler.texture = newTexture->Cast<TextureAsset>()->GetTexture()->GetHandle();

						if (pair.second.previewImage == nullptr)
							pair.second.previewImage = new GUIImage(pair.second.sampler.texture, 50, 50);
						else
							pair.second.previewImage->SetTexture(pair.second.sampler.texture);

						SetDirty(true);
					}
				}

				ImGui::EndCombo();
			}

			if (pair.second.previewImage != nullptr)
				pair.second.previewImage->RenderElement();

			if (ImGui::Button((std::string("Remove###rmvbutton") + pair.first).c_str()))
			{
				delete pair.second.nameField;
				delete pair.second.previewImage;

				_data.samplers.erase(pair.first);
				SetDirty(true);

				break;
			}

			ImGui::Separator();
		}

		// Rename if requested
		for (const auto& pair : rename)
		{
			_data.samplers[pair.second] = _data.samplers[pair.first];
			_data.samplers.erase(pair.first);
		}
	}

	if (_showUnsavedDialog)
	{
		_unsavedDialog->RenderElement();

		if (_unsavedDialog->ShouldClose())
		{
			_showUnsavedDialog = false;
			_unsavedDialog->Reset();
		}
		else if (_unsavedDialog->ShouldSave())
		{
			if (Save())
			{
				SetCloseRequested(true);
			}
			else
			{
				_showUnsavedDialog = false;
				_unsavedDialog->Reset();
			}
		}
		else if (_unsavedDialog->ShouldNotSave())
		{
			SetCloseRequested(true);
		}
	}
}

const std::string& MaterialEditor::GetAssetPath() const
{
	return _assetFile->GetPath();
}

GUIElementType MaterialEditor::GetType()
{
	return GUI_TYPE_MATERIAL_EDITOR;
}

MaterialEditor::~MaterialEditor()
{
	delete _assetFile;
}