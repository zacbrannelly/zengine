#include "ShaderEditor.h"
#include "Directory.h"
#include "File.h"
#include "GUITextField.h"
#include "ShaderBuilder.h"
#include "UnsavedDialog.h"

#include <fstream>
#include <ZEngine-Core/Input/InputManager.h>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <nlohmann/json.hpp>

using namespace nlohmann;

ShaderEditor::ShaderEditor(std::string assetPath) : GUIWindow("Shader Editor - " + assetPath, 800, 400, false)
{
	_assetPath = assetPath;
	_currentPass = 0;
	_showUnsavedDialog = false;
	_unsavedDialog = new UnsavedDialog();

	if (!RetrieveSourcePaths())
		_currentPass = -1;
	else
	{
		_vertSourceField = new GUITextField("###VertexSource");
		_fragSourceField = new GUITextField("###FragmentSource");
		_varySourceField = new GUITextField("###VarySource");

		_vertSourceField->SetMultiline(true);
		_fragSourceField->SetMultiline(true);
		_varySourceField->SetMultiline(true);

		ReadSourceFiles();

		_vertSourceField->SetText(_passes[_currentPass].workingVertSource);
		_fragSourceField->SetText(_passes[_currentPass].workingFragSource);
		_varySourceField->SetText(_passes[_currentPass].workingVarySource);
	}

	SetFlags(ImGuiWindowFlags_MenuBar);
}

bool ShaderEditor::RetrieveSourcePaths()
{
	std::ifstream in(_assetPath, std::ios::in);

	if (!in.is_open())
		return false;

	json root;
	root << in;

	in.close();

	auto passes = root["pass"];

	if (passes.is_array())
	{
		for (const auto& jsonPass : passes)
		{
			ShaderEditorPass pass;

			// Get the path to the shader binaries 
			pass.vertSourcePath = jsonPass["vertex"].get<std::string>();
			pass.fragSourcePath = jsonPass["fragment"].get<std::string>();
			pass.varySourcePath = Directory::GetBasePath(pass.vertSourcePath) + "varying.def.sc";
			
			// Strip ".bin" from the paths (the code should be paired with the binaries)
			pass.vertSourcePath = pass.vertSourcePath.substr(0, pass.vertSourcePath.size() - 4);
			pass.fragSourcePath = pass.fragSourcePath.substr(0, pass.fragSourcePath.size() - 4);

			_passes.push_back(pass);
		}
	}
	else
		return false;

	return true;
}

bool ShaderEditor::ReadSourceFiles()
{
	auto buffer = new char[1000000];
	auto size = 1000000;
	memset(buffer, '\0', size);

	for (auto& pass : _passes)
	{
		File vertFile(pass.vertSourcePath);
		File fragFile(pass.fragSourcePath);
		File varyFile(pass.varySourcePath);

		if (!vertFile.Exists())
			return false;

		if (!fragFile.Exists())
			return false;

		if (!varyFile.Exists())
			return false;

		if (!vertFile.ReadContent(buffer, size))
			return false;

		pass.originalVertSource = std::string(buffer);
		pass.workingVertSource = pass.originalVertSource;

		if (!fragFile.ReadContent(buffer, size))
			return false;

		pass.originalFragSource = std::string(buffer);
		pass.workingFragSource = pass.originalFragSource;

		if (!varyFile.ReadContent(buffer, size))
			return false;

		pass.originalVarySource = std::string(buffer);
		pass.workingVarySource = pass.originalVarySource;
	}

	delete[] buffer;

	return true;
}

bool ShaderEditor::Save()
{
	// Flush current buffers into the current pass data
	_passes[_currentPass].workingVertSource = _vertSourceField->GetText();
	_passes[_currentPass].workingFragSource = _fragSourceField->GetText();
	_passes[_currentPass].workingVarySource = _varySourceField->GetText();

	// Build each pass and write result to binary file
	for (auto& pass : _passes)
	{
		// Write varying definition to file
		if (pass.originalVarySource != pass.workingVarySource)
		{
			File varying(pass.varySourcePath);

			if (!varying.WriteContent(&pass.workingVarySource[0], pass.workingVarySource.size()))
				return false;

			pass.originalVarySource = pass.workingVarySource;
		}

		// Attempt to build the vertex shader and write result to file
		if (pass.originalVertSource != pass.workingVertSource)
		{
			File vertFile(pass.vertSourcePath);

			if (!vertFile.WriteContent(&pass.workingVertSource[0], pass.workingVertSource.size()))
				return false;

			if (!ShaderBuilder::BuildToFile(SB_VERTEX, pass.vertSourcePath, pass.varySourcePath))
			{
				vertFile.WriteContent(&pass.originalVertSource[0], pass.originalVertSource.size());
				return false;
			}

			pass.originalVertSource = pass.workingVertSource;
		}

		// Attempt to build the fragment shader and write result to file
		if (pass.originalFragSource != pass.workingFragSource)
		{
			File fragFile(pass.fragSourcePath);

			if (!fragFile.WriteContent(&pass.workingFragSource[0], pass.workingFragSource.size()))
				return false;

			if (!ShaderBuilder::BuildToFile(SB_FRAGMENT, pass.fragSourcePath, pass.varySourcePath))
			{
				fragFile.WriteContent(&pass.originalFragSource[0], pass.originalFragSource.size());
				return false;
			}

			pass.originalFragSource = pass.workingFragSource;
		}
	}

	SetDirty(false);
	HotReload();

	return true;
}

void ShaderEditor::HotReload()
{
	auto asset = AssetManager::GetInstance()->FindAssetFromPath(_assetPath);

	if (asset != nullptr)
	{
		asset->Load(_assetPath);
	}
}

void ShaderEditor::ProcessInput()
{
	if (ImGui::IsWindowFocused())
	{
		auto input = InputManager::GetInstance();

		// CTRL+S Shortcut - Save & Build
		if (input->GetButtonPressed(BUTTON_KEY_S) && input->ButtonHasModifier(BUTTON_KEY_S, BUTTON_MOD_CONTROL))
		{
			Save();
		}
	}
}

bool ShaderEditor::AllowClose()
{
	if (IsDirty())
		_showUnsavedDialog = true;

	return !IsDirty();
}

const std::string& ShaderEditor::GetAssetPath() const
{
	return _assetPath;
}

void ShaderEditor::RenderInWindow()
{
	if (_currentPass < 0)
	{
		// TODO: Show some error message
		SetCloseRequested(true);
		return;
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save & Build", "CTRL+S"))
			{
				Save();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	int prevPass = _currentPass;
	auto& pass = _passes[_currentPass];

	if (ImGui::BeginCombo("Current Pass", std::to_string(prevPass).c_str()))
	{
		for (int i = 0; i < _passes.size(); i++)
		{
			if (ImGui::Selectable(std::to_string(i).c_str()))
			{
				_currentPass = i;

				if (_currentPass != prevPass)
				{
					const auto& newPass = _passes[i];

					pass.workingVertSource = _vertSourceField->GetText();
					pass.workingFragSource = _fragSourceField->GetText();
					pass.workingVarySource = _varySourceField->GetText();

					_vertSourceField->SetText(newPass.workingVertSource);
					_fragSourceField->SetText(newPass.workingFragSource);
					_varySourceField->SetText(newPass.workingVarySource);

					pass = _passes[_currentPass];
				}
			}
		}

		ImGui::EndCombo();
	}

	if (ImGui::BeginTabBar("Shader Editor Bar"))
	{
		auto codeFont = ImGui::GetIO().Fonts[0].Fonts[1];
		auto contentSize = ImGui::GetContentRegionAvail();

		if (ImGui::BeginTabItem("Vertex Shader"))
		{
			_vertSourceField->SetSize(contentSize.x, contentSize.y);

			ImGui::PushFont(codeFont);
			_vertSourceField->RenderElement();
			ImGui::PopFont();

			ImGui::EndTabItem();

			if (!IsDirty() && _vertSourceField->GetText() != pass.originalVertSource)
			{
				SetDirty(true);
			}
		}

		if (ImGui::BeginTabItem("Fragment Shader"))
		{
			_fragSourceField->SetSize(contentSize.x, contentSize.y);

			ImGui::PushFont(codeFont);
			_fragSourceField->RenderElement();
			ImGui::PopFont();

			ImGui::EndTabItem();

			if (!IsDirty() && _fragSourceField->GetText() != pass.originalFragSource)
			{
				SetDirty(true);
			}
		}

		if (ImGui::BeginTabItem("Varying Definitions"))
		{
			_varySourceField->SetSize(contentSize.x, contentSize.y);

			ImGui::PushFont(codeFont);
			_varySourceField->RenderElement();
			ImGui::PopFont();

			ImGui::EndTabItem();

			if (!IsDirty() && _varySourceField->GetText() != pass.originalVarySource)
			{
				SetDirty(true);
			}
		}

		ImGui::EndTabBar();
	}

	if (_showUnsavedDialog)
	{
		_unsavedDialog->RenderElement();

		if (_unsavedDialog->ShouldClose())
		{
			_showUnsavedDialog = false;
			_unsavedDialog->Reset();
		}
		else if (_unsavedDialog->ShouldNotSave())
		{
			SetCloseRequested(true);
		}
		else if (_unsavedDialog->ShouldSave())
		{
			if (Save())
				SetCloseRequested(true);
			else
			{
				_showUnsavedDialog = false;
				_unsavedDialog->Reset();
			}
		}
	}
}

GUIElementType ShaderEditor::GetType()
{
	return GUI_TYPE_SHADER_EDITOR;
}

ShaderEditor::~ShaderEditor()
{
	delete _vertSourceField;
	delete _fragSourceField;
	delete _varySourceField;
	delete _unsavedDialog;
}
