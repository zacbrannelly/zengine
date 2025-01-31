#pragma once

#include <ZEngine-Core/ImmediateUI/GUIWindow.h>
#include <ZEngine-Core/Rendering/Material.h>
#include <uuid.h>

namespace ZEngine
{
	class File;
	class GUITextField;
	class GUIImage;
	class UnsavedDialog;

	struct MaterialEditorSampler
	{
		Sampler sampler;
		uuids::uuid assetID;
		std::string assetPath;
		GUITextField* nameField;
		GUIImage* previewImage;
	};

	struct MaterialEditorData
	{
		std::string name;
		uuids::uuid shaderID;
		std::string shaderPath;

		std::map<std::string, Uniform> uniforms;
		std::map<std::string, MaterialEditorSampler> samplers;

		std::map<std::string, GUITextField*> uniformNameFields;
	};

	class MaterialEditor : public GUIWindow
	{
	public:
		MaterialEditor(std::string assetPath);
		~MaterialEditor();

		void ProcessInput() override;
		void RenderInWindow() override;
		bool AllowClose() override;
		GUIElementType GetType() override;

		bool LoadData();
		bool Save();
		void HotReload();

		const std::string& GetAssetPath() const;

	private:
		File* _assetFile;
		MaterialEditorData _data;
		bool _showUnsavedDialog;
		UnsavedDialog* _unsavedDialog;
	};
}
