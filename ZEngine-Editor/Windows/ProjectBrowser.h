#pragma once

#include <ZEngine-Core/ImmediateUI/GUIWindow.h>
#include <ZEngine-Core/Utilities/File.h>
#include <ZEngine-Core/Utilities/Directory.h>
#include <vector>

namespace ZEngine
{
	class Editor;
	class Project;
	class AssetManager;

	class ProjectBrowser : public GUIWindow
	{
	public:
		ProjectBrowser(Editor* editor);
		~ProjectBrowser();

		void ProcessInput() override;
		void RenderInWindow() override;
		void RenderElement() override;

		void NavigateTo(const std::string& path);
		void OpenFile(const std::string& path);

	private:
		bool RenderFile(
			const std::string& basename,
			bool selected,
			bool isDirectory = false,
			bool isImported = false,
			ImTextureID previewTextureId = 0
		);
		void RenderDirectoryContextMenu();
		void RenderFileContextMenu(File const& file, bool isImported);
		void RenderGeneralContextMenu();
		Directory GetCurrentDirectory() const;
		
		Editor* _editor;
		AssetManager* _assetManager;
		Project* _viewingProject;
		std::string _currentDirectory;
		int _selectedItem;
		std::vector<File> _files;
		std::vector<Directory> _directories;
	};
}
