#pragma once

#include "Component.h"

#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <bgfx/bgfx.h>

class Graphics;

class Camera : public Component
{
public:
	Camera();
	~Camera();

	enum ProjectionMode
	{
		ORTHOGRAPHIC, PERSPECTIVE
	};

	void Init() override;
	void Update() override;
	void Render(int viewId) override;

	void SetViewId(int viewId);
	int GetViewId() const;

	void SetClearFlags(uint16_t flags);
	void SetClearColor(float r, float g, float b, float a);
	void SetClearColor(const glm::vec4& color);
	const glm::vec4& GetClearColor() const;

	void SetViewport(int x, int y, int width, int height);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	float GetAspectRatio() const;

	void SetProjectionMode(ProjectionMode mode);
	ProjectionMode GetProjectionMode() const;

	void SetRenderToTexture(bool renderToTexture);
	bool IsRenderingToTexture() const;
	bgfx::TextureHandle GetRenderTexture() const;

	void SetFieldOfView(float fov);
	float GetFieldOfView() const;

	void SetOrthoSize(float size);
	float GetOrthoSize() const;

	void SetNear(float near);
	void SetFar(float far);
	float GetNear() const;
	float GetFar() const;

private:
	Graphics* _graphics;
	ProjectionMode _projMode;

	// Global settings
	int _viewId;
	uint16_t _clearFlags;
	glm::vec4 _clearColor;

	float _zNear, _zFar;
	glm::vec4 _viewport;

	bgfx::FrameBufferHandle _frameBuffer;
	bgfx::TextureHandle _renderTexture;

	// Ortho settings
	float _orthoSize;

	// Perspective settings
	float _fov;

public:
	static ZObject* CreateInstance(std::string name, ObjectType type);
};

