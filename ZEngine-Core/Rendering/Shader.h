#pragma once

#include "../Map/Objects/ZObject.h"
#include <bgfx/bgfx.h>

class Shader : public ZObject
{
public:
	Shader(std::string name);
	~Shader();

	bool Load(std::string vertPath, std::string fragPath);

	const bgfx::ProgramHandle& GetHandle() const;
	const bgfx::ShaderHandle& GetVertexShader() const;
	const bgfx::ShaderHandle& GetFragmentShader() const;

private:
	const bgfx::Memory* LoadFile(const std::string& path) const;

	bgfx::ShaderHandle _vertexShader;
	bgfx::ShaderHandle _fragmentShader;
	bgfx::ProgramHandle _program;
	bool _isLoaded;
	 
public:
	static ZObject* CreateInstance(std::string name, ObjectType type);

};

