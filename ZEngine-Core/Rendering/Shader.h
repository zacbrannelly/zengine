#pragma once

#include "../Map/Objects/ZObject.h"
#include <bgfx/bgfx.h>
#include <vector>

struct Pass
{
	bgfx::ShaderHandle vertexShader;
	bgfx::ShaderHandle fragmentShader;
	bgfx::ProgramHandle program;
	uint64_t state;
};

class Shader : public ZObject
{
public:
	Shader(std::string name);
	~Shader();

	bool Load(std::string vertPath, std::string fragPath, uint64_t = 0, int pass = 0);

	const bgfx::ProgramHandle& GetHandle(int pass = 0) const;
	const bgfx::ShaderHandle& GetVertexShader(int pass = 0) const;
	const bgfx::ShaderHandle& GetFragmentShader(int pass = 0) const;

	const std::vector<Pass>& GetPasses() const;

private:
	const bgfx::Memory* LoadFile(const std::string& path) const;
	std::vector<Pass> _pass;
	bool _isLoaded;
	 
public:
	static ZObject* CreateInstance(std::string name, ObjectType type);

};

