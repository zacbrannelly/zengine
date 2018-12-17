#pragma once

#include <vector>
#include <string>

#include "GUIWindow.h"

class Editor;
class AssetManager;
class File;
struct CatalogEntry;

class AssetExplorer : public GUIWindow
{
public:
	AssetExplorer(Editor* editor);
	~AssetExplorer();

	void ProcessInput() override;
	void RenderInWindow() override;

private:
	void RenderAssetButtons(const std::vector<CatalogEntry>& items);
	
	Editor* _editor;
	AssetManager* _manager;
	std::string _selectedName;
};

