#include "Graphics.h"
#include "../Display/Display.h"
#include "VertexBuffer.h"
#include "DynamicVertexBuffer.h"
#include "IndexBuffer.h"
#include "DynamicIndexBuffer.h"
#include "Material.h"
#include "Shader.h"

#include <GLFW\glfw3.h>
#include <bgfx\platform.h>

#include <iostream>

using namespace bgfx;

uint32_t convertRGBToInt(int r, int g, int b, int a)
{
	return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (a & 0xff);
}

Graphics::Graphics() : _initialized(false)
{

}

bool Graphics::Init(Display* display)
{
	if (display == nullptr || display->GetHandle() == nullptr)
	{
		std::cout << "GRAPHICS: Display was null or not created, it is needed to create a rendering context!" << std::endl;
		return false;
	}

	// Hook the window up to BGFX rendering API
	PlatformData pd;
	pd.backBuffer = nullptr;
	pd.backBufferDS = nullptr;
	pd.context = nullptr;
	pd.ndt = nullptr;
	pd.nwh = display->GetWin32Handle();

	setPlatformData(pd);

	// Setup BGFX initialization parameters
	bgfx::Init i;
	i.allocator = nullptr;
	i.callback = nullptr;
	i.debug = false;
	i.profile = false;
	i.type = RendererType::OpenGL;
	
	// Setup resolution to give to BGFX
	Resolution res;
	res.width = display->GetWidth();
	res.height = display->GetHeight();
	i.resolution = res;

	if (!init(i))
	{
		std::cout << "GRAPHICS: Failed to initialize BGFX (Rendering context)" << std::endl;
		return false;
	}

	_initialized = true;

	return true;
}

const bool Graphics::IsInitialized() const
{
	return _initialized;
}

void Graphics::SetFrameBuffer(int viewId, FrameBufferHandle fbo)
{
	setViewFrameBuffer(viewId, fbo);
}

void Graphics::Reset(int width, int height, uint32_t flags)
{
	reset(width, height, flags);
}

void Graphics::Clear(int viewId)
{
	Clear(viewId, 0, 0, 0, 1);
}

void Graphics::Clear(int viewId, int r, int g, int b, int a)
{
	Clear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, r, g, b, a);
}

void Graphics::Clear(int viewId, uint16_t flags, int r, int g, int b, int a)
{
	setViewClear(viewId, flags, convertRGBToInt(r, g, b, a), 1.0f, 0);
}

void Graphics::Viewport(int viewId, int x, int y, int width, int height)
{
	setViewRect(viewId, x, y, width, height);
}

void Graphics::ViewMode(int viewId, bgfx::ViewMode::Enum mode)
{
	setViewMode(viewId, mode);
}

void Graphics::ViewTransform(int viewId, const glm::mat4& projMatrix, const glm::mat4& viewMatrix)
{
	setViewTransform(viewId, &viewMatrix[0][0], &projMatrix[0][0]);
}

void Graphics::Touch(int viewId)
{
	touch(viewId);
}

bgfx::UniformHandle Graphics::CreateUniform(const std::string & name, bgfx::UniformType::Enum type, uint16_t numElements)
{
	return createUniform(name.c_str(), type, numElements);
}

void Graphics::SetVertexBuffer(uint8_t stream, VertexBuffer* buffer)
{
	if (buffer != nullptr)
		setVertexBuffer(stream, buffer->GetHandle());
}

void Graphics::SetIndexBuffer(IndexBuffer* buffer)
{
	if (buffer != nullptr)
		setIndexBuffer(buffer->GetHandle());
}

void Graphics::SetTransform(const glm::mat4 & transform)
{
	setTransform(&transform[0][0]);
}

void Graphics::SetState(uint64_t state)
{
	setState(state);
}

void Graphics::SetUniform(const std::string& name, bgfx::UniformType::Enum type, const void * data, uint16_t numElements)
{
	auto& uniform = createUniform(name.c_str(), type, numElements);
	setUniform(uniform, data, numElements);
}

void Graphics::SetUniform(bgfx::UniformHandle& uniform, const void* data, uint16_t numElements)
{
	setUniform(uniform, data, numElements);
}

void Graphics::SetTexture(uint8_t stage, bgfx::UniformHandle & sampler, bgfx::TextureHandle & handle)
{
	setTexture(stage, sampler, handle);
}

void Graphics::SetTexture(uint8_t stage, bgfx::UniformHandle& sampler, bgfx::TextureHandle & handle, uint32_t flags)
{
	setTexture(stage, sampler, handle, flags);
}

void Graphics::Submit(int viewId, Material* material, uint64_t state)
{
	if (material == nullptr || material->GetShader() == nullptr) return;

	material->Apply();

	// Submit multiple draw calls for a multi-pass shader
	for (auto& pass : material->GetShader()->GetPasses())
	{
		SetState(state | pass.state);
		Submit(viewId, pass.program);
	}
}

void Graphics::Submit(int viewId, bgfx::ProgramHandle program)
{
	submit(viewId, program);
}

void Graphics::Render()
{
	frame();
}

FrameBufferHandle Graphics::CreateFrameBuffer(int width, int height)
{
	// NOTE: The second texture is a depth buffer, this is needed!!!!
	bgfx::TextureHandle buffers[2];
	buffers[0] = createTexture2D(width, height, false, 1, bgfx::TextureFormat::BGRA8);
	buffers[1] = createTexture2D(width, height, false, 1, bgfx::TextureFormat::D24);

	return createFrameBuffer(2, buffers, true);
}

TextureHandle Graphics::GetFrameBufferTexture(FrameBufferHandle fbo)
{
	return getTexture(fbo);
}

void Graphics::DeleteFrameBuffer(FrameBufferHandle fbo)
{
	bgfx::destroy(fbo);
}

void Graphics::Shutdown()
{
	shutdown();
}
