#pragma once
#include "../Misc/Singleton.h"
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

class Display;

class Graphics : public Singleton<Graphics>
{
public:
	Graphics();

	bool Init(Display* display);
	void Shutdown();

	void Reset(int width, int height, uint32_t resetFlags);
	void SetFrameBuffer(int viewId, bgfx::FrameBufferHandle fbo);
	void Clear(int viewId);
	void Clear(int viewId, int r, int g, int b, int a);
	void Clear(int viewId, uint16_t flags, int r, int g, int b, int a);
	void Viewport(int viewId, int x, int y, int width, int height);
	void ViewMode(int viewId, bgfx::ViewMode::Enum mode);
	void ViewTransform(int viewId, const glm::mat4& projMatrix, const glm::mat4& viewMatrix);
	void Touch(int viewId);

	void SetTransform(const glm::mat4& transform);
	void SetState(uint64_t state);
	void Submit(int viewId, bgfx::ProgramHandle program);

	bgfx::FrameBufferHandle CreateFrameBuffer(int width, int height);
	bgfx::TextureHandle GetFrameBufferTexture(bgfx::FrameBufferHandle fbo);
	void DeleteFrameBuffer(bgfx::FrameBufferHandle fbo);

	void Render();

	const bool IsInitialized() const;
private:
	bool _initialized;
};

