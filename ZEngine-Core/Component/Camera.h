#pragma once

#include "Component.h"

#ifdef __APPLE__
/**
 * @brief Make GLM use the same coordinate system as Metal (depth 0 to 1)
 */
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif

#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <bgfx/bgfx.h>
#include "../Utilities/JsonHelpers.h"

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
	uint16_t GetClearFlags() const;
	void SetClearColor(float r, float g, float b, float a);
	void SetClearColor(const glm::vec4& color);
	const glm::vec4& GetClearColor() const;

	void SetViewport(glm::vec4 viewport);
	void SetViewport(int x, int y, int width, int height);
	int GetViewportX() const;
	int GetViewportY() const;
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

	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewMatrix() const;

private:
	Graphics* _graphics;
	ProjectionMode _projection;

	// Global settings
	int _viewId;
	uint16_t _clearFlags;
	glm::vec4 _clearColor;

	float _zNear, _zFar;
	glm::vec4 _viewport;

	bool _renderToTexture;
	bgfx::FrameBufferHandle _frameBuffer;
	bgfx::TextureHandle _renderTexture;

	// Ortho settings
	float _orthoSize;

	// Perspective settings
	float _fieldOfView;

#ifndef SWIG
	// Allow serialization / deserialization
	JSON_SCHEMA_BEGIN(Camera)
		JSON_TO_SETTER_OPTIONAL(viewId,          SetViewId,          int)
		JSON_TO_SETTER_OPTIONAL(clearFlags,      SetClearFlags,      uint16_t)
		JSON_TO_SETTER_OPTIONAL(clearColor,      SetClearColor,      glm::vec4)
		JSON_TO_SETTER_OPTIONAL(viewport,        SetViewport,        glm::vec4)
		JSON_TO_SETTER_OPTIONAL(projection,      SetProjectionMode,  ProjectionMode)
		JSON_TO_SETTER_OPTIONAL(renderToTexture, SetRenderToTexture, bool)
		JSON_TO_SETTER_OPTIONAL(fieldOfView,     SetFieldOfView,     float)
		JSON_TO_SETTER_OPTIONAL(orthoSize,       SetOrthoSize,       float)
		JSON_TO_SETTER_OPTIONAL(zNear,           SetNear,            float)
		JSON_TO_SETTER_OPTIONAL(zFar,            SetFar,             float)
	JSON_SCHEMA_END()
#endif

public:
	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

	static ObjectType GetStaticType()
	{
		return CAMERA;	
	}
};

