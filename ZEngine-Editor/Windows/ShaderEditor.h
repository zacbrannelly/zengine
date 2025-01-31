#pragma once

#include <vector>
#include <ZEngine-Core/ImmediateUI/GUIWindow.h>

namespace ZEngine
{
	class UnsavedDialog;
	class GUITextField;

	struct ShaderEditorPass
	{
		std::string originalVertSource;
		std::string originalFragSource;
		std::string originalVarySource;

		std::string workingVertSource;
		std::string workingFragSource;
		std::string workingVarySource;

		std::string vertSourcePath;
		std::string fragSourcePath;
		std::string varySourcePath;
	};

	class ShaderEditor : public GUIWindow
	{
	public:
		ShaderEditor(std::string assetPath);
		~ShaderEditor();

		void ProcessInput() override;
		void RenderInWindow() override;
		GUIElementType GetType() override;

		bool AllowClose() override;

		const std::string& GetAssetPath() const;

	private:
		bool RetrieveSourcePaths();
		bool ReadSourceFiles();
		bool Save();
		void HotReload();

		std::string _assetPath;
		int _currentPass;
		std::vector<ShaderEditorPass> _passes;
		bool _showUnsavedDialog;
		UnsavedDialog* _unsavedDialog;

		GUITextField* _vertSourceField;
		GUITextField* _fragSourceField;
		GUITextField* _varySourceField;
	};
}
