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

	void Reset(int width, int height, int resetFlags);
	void SetFrameBuffer(int viewId, bgfx::FrameBufferHandle fbo);
	void Clear(int viewId);
	void Clear(int viewId, int r, int g, int b, int a);
	void Clear(int viewId, int flags, int r, int g, int b, int a);
	void Viewport(int viewId, int x, int y, int width, int height);
	void ViewMode(int viewId, bgfx::ViewMode::Enum mode);
	void ViewTransform(int viewId, const glm::mat4& projMatrix, const glm::mat4& viewMatrix);
	void Touch(int viewId);

	bgfx::FrameBufferHandle CreateFrameBuffer(int width, int height);
	bgfx::TextureHandle GetFrameBufferTexture(bgfx::FrameBufferHandle fbo);
	void DeleteFrameBuffer(bgfx::FrameBufferHandle fbo);

	void Render();

	const bool IsInitialized() const;
private:
	bool _initialized;
};

