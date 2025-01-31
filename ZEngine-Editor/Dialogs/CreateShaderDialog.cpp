#include "CreateShaderDialog.h"
#include "BrowserDialog.h"
#include <ZEngine-Core/ImmediateUI/GUITextField.h>
#include <ZEngine-Core/Utilities/File.h>
#include <ZEngine-Core/Utilities/Directory.h>
#include "../Builders/ShaderBuilder.h"

#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <sstream>
#include <algorithm>
#include <nlohmann/json.hpp>

using namespace ZEngine;

CreateShaderDialog::CreateShaderDialog(std::string basePath) : GUIDialog("Create Shader", 515, 160, true)
{
	_basePath = basePath;
	_nameField = new GUITextField("Name");
	_assetPathField = new GUITextField("Asset Path");

	_assetPathField->SetText(basePath);

	_browser = new BrowserDialog("./", BROWSER_SAVE_FILE);
	_browser->SetFilter({ "shader" });
}

bool CreateShaderDialog::Validate()
{
	auto name = _nameField->GetText();

	if (name == "")
		return false;

	if (std::all_of(name.begin(), name.end(), [](const auto& c) { return c == ' '; }))
		return false;
	
	if (!ValidatePath(_assetPathField->GetText(), "shader"))
		return false;

	return true;
}

bool CreateShaderDialog::ValidatePath(const std::string& path, std::string prefExtension)
{
	if (path == "")
		return false;

	if (std::all_of(path.begin(), path.end(), [](const auto& c) { return c == ' '; }))
		return false;

	if (Directory::GetExtension(path) != prefExtension)
		return false;

	return true;
}

std::string CreateShaderDialog::GenerateVertexCode() const
{
	std::stringstream ss;

	ss << "$input a_position, a_color0, a_texcoord0" << std::endl;
	ss << "$output v_color0, v_texcoord0" << std::endl;
	ss << std::endl;

	ss << "#include \"lib/bgfx_shader.sh\"" << std::endl;
	ss << std::endl;

	ss << "void main()" << std::endl;
	ss << "{" << std::endl;
	ss << "\t" << "gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));" << std::endl;
	ss << "\t" << "v_color0 = a_color0;" << std::endl;
	ss << "\t" << "v_texcoord0 = a_texcoord0;" << std::endl;
	ss << "}" << std::endl;

	return ss.str();
}

std::string CreateShaderDialog::GenerateFragmentCode() const
{
	std::stringstream ss;

	ss << "$input v_color0, v_texcoord0" << std::endl;
	ss << std::endl;

	ss << "uniform sampler2D texColor;" << std::endl;
	ss << std::endl;

	ss << "void main()" << std::endl;
	ss << "{" << std::endl;
	ss << "\t" << "gl_FragColor = texture2D(texColor, v_texcoord0) * v_color0;" << std::endl;
	ss << "}" << std::endl;

	return ss.str();
}

std::string CreateShaderDialog::GenerateVaryingDefCode() const
{
	std::stringstream ss;

	ss << "vec4 v_color0 : COLOR0 = vec4(1.0, 0.0, 0.0, 1.0);" << std::endl;
	ss << "vec3 v_normal : NORMAL = vec3(0.0, 0.0, 1.0);" << std::endl;
	ss << "vec2 v_texcoord0 : TEXCOORD0 = vec2(0.0, 0.0);" << std::endl;
	ss << "vec3 v_fragPos : TEXCOORD1 = vec3(0, 0, 0);" << std::endl;
	ss << std::endl;

	ss << "vec3 a_position : POSITION;" << std::endl;
	ss << "vec4 a_color0 : COLOR0;" << std::endl;
	ss << "vec2 a_texcoord0 : TEXCOORD0;" << std::endl;
	ss << "vec3 a_normal : NORMAL;" << std::endl;

	return ss.str();
}

std::string CreateShaderDialog::GenerateJSON() const
{
	using namespace nlohmann;

	json root;

	root["name"] = _nameField->GetText();
	auto& passes = root["pass"] = json::array_t();

	for (int i = 0; i < _numberOfPasses; i++)
	{
		json::object_t pass;
		pass["vertex"] = _basePath + "vs_" + _nameField->GetText() + "." + std::to_string(i) + ".sc.bin";
		pass["fragment"] = _basePath + "fs_" + _nameField->GetText() + "." + std::to_string(i) + ".sc.bin";

		passes.push_back(pass);
	}

	return root.dump(4);
}

bool CreateShaderDialog::Create(bool import)
{
	if (Validate())
	{
		_basePath = Directory::GetBasePath(_assetPathField->GetText());

		for (int i = 0; i < _numberOfPasses; i++)
		{
			auto vertCode = GenerateVertexCode();
			auto fragCode = GenerateFragmentCode();
			auto varyCode = GenerateVaryingDefCode();

			File vertFile(_basePath + "vs_" + _nameField->GetText() + "." + std::to_string(i) + ".sc");
			File fragFile(_basePath + "fs_" + _nameField->GetText() + "." + std::to_string(i) + ".sc");
			File varyFile(_basePath + "varying.def." + _nameField->GetText() + "." + std::to_string(i) + ".sc");

			if (!vertFile.WriteContent(&vertCode[0], vertCode.size()))
				return false;

			if (!fragFile.WriteContent(&fragCode[0], fragCode.size()))
				return false;

			if (!varyFile.WriteContent(&varyCode[0], varyCode.size()))
				return false;

			if (!ShaderBuilder::BuildToFile(SB_VERTEX, vertFile.GetPath(), varyFile.GetPath()))
				return false;

			if (!ShaderBuilder::BuildToFile(SB_FRAGMENT, fragFile.GetPath(), varyFile.GetPath()))
				return false;
		}

		File jsonFile(_assetPathField->GetText());
		auto jsonCode = GenerateJSON();

		if (!jsonFile.WriteContent(&jsonCode[0], jsonCode.size()))
			return false;

		if (import)
		{
			auto catalog = AssetManager::GetInstance()->GetCatalog();

			if (catalog != nullptr)
			{
				catalog->RegisterAsset(jsonFile.GetPath(), SHADER_ASSET);
				catalog->SaveCatalog(catalog->GetLastCatalogPath());
			}
		}

		return true;
	}

	return false;
}

void CreateShaderDialog::ProcessInput()
{
	if (GetResult() == DIALOG_RESULT_CLOSE)
		Close();

	if (_browser->IsVisible())
	{
		if (_browser->GetResult() == DIALOG_RESULT_CLOSE)
		{
			_browser->SetUserData(nullptr);
			_browser->Hide();
		}
		else if (_browser->GetResult() == DIALOG_RESULT_OK)
		{
			_assetPathField->SetText(_browser->GetFile().GetRelativePath());
			_nameField->SetText(_browser->GetFile().GetName());
			_basePath = _browser->GetDirectory().GetPathRelativeTo();

			_browser->Hide();
		}
	}

	if (_nameField->GetText() != "")
	{
		auto name = _nameField->GetText();
		_assetPathField->SetText(_basePath + name + ".shader");
	}
	else
	{
		_assetPathField->SetText(_basePath);
	}
}

void CreateShaderDialog::RenderInWindow()
{
	_browser->RenderElement();

	_nameField->RenderElement();

	if (ImGui::Button("...###..0"))
	{
		_browser->SetUserData(_assetPathField);
		_browser->SetSaveExtension("shader");
		_browser->Show();
	}

	ImGui::SameLine();
	_assetPathField->RenderElement();

	ImGui::Separator();

	ImGui::InputInt("Number of passes", &_numberOfPasses);

	if (_numberOfPasses <= 0)
		_numberOfPasses = 1;

	ImGui::Separator();
	
	if (ImGui::Button("Create"))
	{
		if (Create(false))
			Close();
	}

	ImGui::SameLine();
	if (ImGui::Button("Create & Import"))
	{
		if (Create(true))
			Close();
	}

	ImGui::SameLine();
	if (ImGui::Button("Cancel"))
		Close();
}

CreateShaderDialog::~CreateShaderDialog()
{
	delete _nameField;
	delete _assetPathField;
	delete _browser;
}