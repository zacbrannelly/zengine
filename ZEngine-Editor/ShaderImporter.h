#pragma once
#include <vector>
#include "GUIWindow.h"
#include "GUITextField.h"

#define SHADER_IMPORTER_BUFFER_SIZE 1000

struct ShaderImporterPass
{
	ShaderImporterPass()
	{
		vertShaderPathBuffer = new char[SHADER_IMPORTER_BUFFER_SIZE];
		fragShaderPathBuffer = new char[SHADER_IMPORTER_BUFFER_SIZE];
		varyingDefPathBuffer = new char[SHADER_IMPORTER_BUFFER_SIZE];

		memset(vertShaderPathBuffer, '\0', SHADER_IMPORTER_BUFFER_SIZE);
		memset(fragShaderPathBuffer, '\0', SHADER_IMPORTER_BUFFER_SIZE);
		memset(varyingDefPathBuffer, '\0', SHADER_IMPORTER_BUFFER_SIZE);
		
		// Set default varying def path
		std::string defaultVarying = "varying.def.sc";
		memcpy(varyingDefPathBuffer, &defaultVarying[0], defaultVarying.size());
	}

	std::string vertShaderPath;
	std::string fragShaderPath;
	std::string varyingDefPath;

	char* vertShaderPathBuffer;
	char* fragShaderPathBuffer;
	char* varyingDefPathBuffer;
};

class ShaderImporter : public GUIWindow
{
public:
	ShaderImporter(Container* owner);
	ShaderImporter(Container* owner, std::string filepath);
	~ShaderImporter();

	void ProcessInput() override;
	void RenderInWindow() override;
	void RenderElement() override;

	bool Validate();
	bool Create();

private:
	GUITextField* _nameTextField;
	std::string _unknownFile;
	bool _pathTypeKnown;
	Container* _owner;
	std::vector<ShaderImporterPass> _passes;
};