#pragma once

#include "../Map/Objects/ZObject.h"
#include <bgfx/bgfx.h>
#include <vector>

namespace ZEngine
{
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

		bool Load(std::string vertPath, std::string fragPath, uint64_t state = 0, int pass = 0);
		bool Load(uint8_t* vertData, uint32_t vertSize, uint8_t* fragData, uint32_t fragSize, uint64_t state = 0, int pass = 0);
		void Release();

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
		
		static ObjectType GetStaticType()
		{
			return SHADER;
		}
	};
}
