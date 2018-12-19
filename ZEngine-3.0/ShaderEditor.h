#pragma once

#include <vector>
#include "GUIWindow.h"

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

private:
	bool RetrieveSourcePaths();
	bool ReadSourceFiles();
	bool Save();

	std::string _assetPath;
	int _currentPass;
	std::vector<ShaderEditorPass> _passes;

	GUITextField* _vertSourceField;
	GUITextField* _fragSourceField;
	GUITextField* _varySourceField;
};
