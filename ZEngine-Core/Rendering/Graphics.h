#pragma once
#include "../Misc/Singleton.h"
#include "../Logging/Logger.h"
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include <string>

/**
View ID determins the rendering order of the views in ascending order.
Make sure to use the below macros to define the view IDs for your views.
*/
#define ZENGINE_EDITOR_SHADOW_PASS_BASE_VIEW_ID 0
#define ZENGINE_EDITOR_SHADOW_PASS_VIEW_ID(viewId) (ZENGINE_EDITOR_SHADOW_PASS_BASE_VIEW_ID + viewId)

#define ZENGINE_EDITOR_BASE_VIEW_ID 20
#define ZENGINE_EDITOR_VIEW_ID(viewId) (ZENGINE_EDITOR_BASE_VIEW_ID + viewId)

#define ZENGINE_SHADOW_PASS_BASE_VIEW_ID 40
#define ZENGINE_SHADOW_PASS_VIEW_ID(viewId) (ZENGINE_SHADOW_PASS_BASE_VIEW_ID + viewId)

#define ZENGINE_VIEW_ID_BASE 60
#define ZENGINE_VIEW_ID(viewId) (ZENGINE_VIEW_ID_BASE + viewId)

namespace ZEngine
{
	class Display;
	class VertexBuffer;
	class IndexBuffer;
	class DynamicVertexBuffer;
	class DynamicIndexBuffer;
	class Material;
	class FrameBuffer;

	class Graphics : public Singleton<Graphics>
	{
	public:
		Graphics();

		bool Init(Display* display);
		bool Init(void* nativeWindowHandle, int width, int height);
		void Shutdown();

		void Reset(int width, int height, uint32_t resetFlags);
		void SetFrameBuffer(int viewId, FrameBuffer* frameBuffer);
		void Clear(int viewId);
		void Clear(int viewId, int r, int g, int b, int a);
		void Clear(int viewId, uint16_t flags, int r, int g, int b, int a);
		void Viewport(int viewId, int x, int y, int width, int height);
		void ViewMode(int viewId, bgfx::ViewMode::Enum mode);
		void ViewTransform(int viewId, const glm::mat4& projMatrix, const glm::mat4& viewMatrix);
		void Touch(int viewId);

		bgfx::UniformHandle CreateUniform(const std::string& name, bgfx::UniformType::Enum type, uint16_t numElements);
		void DestroyUniform(const bgfx::UniformHandle& uniform);

		void SetVertexBuffer(uint8_t stream, VertexBuffer* buffer);
		void SetIndexBuffer(IndexBuffer* buffer);
		void SetTransform(const glm::mat4& transform);
		void SetState(uint64_t state);
		void SetUniform(const std::string& name, bgfx::UniformType::Enum type, const void* data, uint16_t numElements);
		void SetUniform(bgfx::UniformHandle& uniform, const void* data, uint16_t numElements);
		void SetTexture(uint8_t stage, bgfx::UniformHandle& sampler, bgfx::TextureHandle& handle);
		void SetTexture(uint8_t stage, bgfx::UniformHandle& sampler, bgfx::TextureHandle& handle, uint32_t flags);
		void Submit(int viewId, bgfx::ProgramHandle program);

		bgfx::TextureHandle CreateTexture2D(int width, int height, bool hasMips, uint16_t numLayers, bgfx::TextureFormat::Enum format, uint64_t flags);
		void DestroyTexture(bgfx::TextureHandle texture);

		bgfx::FrameBufferHandle CreateFrameBuffer(bgfx::Attachment textureAttachment);
		bgfx::FrameBufferHandle CreateFrameBuffer(int width, int height);
		bgfx::FrameBufferHandle CreateDepthFrameBuffer(int width, int height);
		bgfx::TextureHandle GetFrameBufferTexture(bgfx::FrameBufferHandle fbo);
		void DeleteFrameBuffer(bgfx::FrameBufferHandle fbo);

		void Render();

		const bool IsInitialized() const;
	private:
		bool _initialized;
		Logger _logger { "Graphics" };
	};
}
