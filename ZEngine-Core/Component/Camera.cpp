#include "Camera.h"
#include "../Rendering/Graphics.h"
#include "../Map/Objects/Entity.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace std;

Camera::Camera() : Component("Camera", ObjectType::CAMERA)
{
	_projMode = ORTHOGRAPHIC;
	_frameBuffer = BGFX_INVALID_HANDLE;
	_renderTexture = BGFX_INVALID_HANDLE;
	_clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
	_zNear = 0.001f;
	_zFar = 100.0f;
	_viewport = { 0, 0, 1024, 600 };
	_clearColor = { 1, 1, 1, 1 };
	_orthoSize = 5.0f;
	_fov = glm::radians(60.0f);
}

void Camera::Init()
{
	_graphics = Graphics::GetInstance();
}

void Camera::Update()
{
	// Must be implemented for components
}

void Camera::Render(int viewId)
{
	// Setup the view
	_graphics->SetFrameBuffer(_viewId, _frameBuffer);
	_graphics->ViewMode(_viewId, bgfx::ViewMode::Default);
	_graphics->Viewport(_viewId, (int)(_viewport.x + 0.5f), (int)(_viewport.y + 0.5f), (int)(_viewport.z + 0.5f), (int)(_viewport.w + 0.5f));
	_graphics->Clear(_viewId, _clearFlags, (int)(_clearColor.r * 255.0f + 0.5f), (int)(_clearColor.g * 255.0f + 0.5f), (int)(_clearColor.b * 255.0f + 0.5f), (int)(_clearColor.a * 255.0f + 0.5f));

	// Calculate projection matrix based off projection mode
	glm::mat4 projMatrix(1.0f);
	auto aspectRatio = GetAspectRatio();

	if (_projMode == Camera::ProjectionMode::ORTHOGRAPHIC)
	{
		// Create orthogonal projection with _orthoSize being the half vertical height
		projMatrix = glm::ortho<float>(-aspectRatio * _orthoSize, aspectRatio * _orthoSize, -_orthoSize, _orthoSize, _zNear, _zFar);
	}
	else
	{
		// Create perspective projection (3D) with the field of view (zoom)
		projMatrix = glm::perspective<float>(_fov, aspectRatio, _zNear, _zFar);
	}

	// View matrix is the inverse of the camera's world transformation matrix
	auto viewMatrix = glm::inverse(GetOwner()->GetTransform()->GetWorldTransformMatrix());

	// Upload projection and view matrices to the GPU
	_graphics->ViewTransform(_viewId, projMatrix, viewMatrix);

	// Ensure the screen is cleared when nothing is being drawn (for testing purposes mostly, can remove later)
	_graphics->Touch(_viewId);
}

void Camera::SetClearFlags(uint16_t flags)
{
	_clearFlags = flags;
}

uint16_t Camera::GetClearFlags() const
{
	return _clearFlags;
}

void Camera::SetClearColor(float r, float g, float b, float a)
{
	_clearColor = glm::vec4(r, g, b, a);
}

void Camera::SetClearColor(const glm::vec4& color)
{
	_clearColor = color;
}

const glm::vec4& Camera::GetClearColor() const
{
	return _clearColor;
}

void Camera::SetViewId(int viewId)
{
	_viewId = viewId;
}

int Camera::GetViewId() const
{
	return _viewId;
}

void Camera::SetViewport(int x, int y, int width, int height)
{
	_viewport = { x, y, width, height };

	if (IsRenderingToTexture())
	{
		bgfx::destroy(_frameBuffer);

		_frameBuffer = _graphics->CreateFrameBuffer(_viewport.z, _viewport.w);
		_renderTexture = _graphics->GetFrameBufferTexture(_frameBuffer);
	}
}

int Camera::GetViewportX() const
{
	return (int)(_viewport.x + 0.5f);
}

int Camera::GetViewportY() const
{
	return (int)(_viewport.y + 0.5f);
}

int Camera::GetViewportWidth() const
{
	return (int)(_viewport.z + 0.5f);
}

int Camera::GetViewportHeight() const
{
	return (int)(_viewport.w + 0.5f);
}

float Camera::GetAspectRatio() const
{
	return _viewport.z / _viewport.w;
}

void Camera::SetProjectionMode(Camera::ProjectionMode mode)
{
	_projMode = mode;
}

Camera::ProjectionMode Camera::GetProjectionMode() const
{
	return _projMode;
}

void Camera::SetRenderToTexture(bool renderToTexture)
{
	if (renderToTexture && !IsRenderingToTexture())
	{
		_frameBuffer = _graphics->CreateFrameBuffer(_viewport.z, _viewport.w);
		_renderTexture = _graphics->GetFrameBufferTexture(_frameBuffer);
	}
	else if (!renderToTexture && IsRenderingToTexture())
	{
		_graphics->DeleteFrameBuffer(_frameBuffer);
		_frameBuffer = BGFX_INVALID_HANDLE;
	}
}

bool Camera::IsRenderingToTexture() const
{
	return _frameBuffer.idx != bgfx::kInvalidHandle;
}

bgfx::TextureHandle Camera::GetRenderTexture() const
{
	return _renderTexture;
}

void Camera::SetFieldOfView(float fov)
{
	_fov = glm::radians(fov);
}

float Camera::GetFieldOfView() const
{
	return _fov;
}

void Camera::SetOrthoSize(float size)
{
	_orthoSize = size;
}

float Camera::GetOrthoSize() const
{
	return _orthoSize;
}

void Camera::SetNear(float near)
{
	_zNear = near;
}

void Camera::SetFar(float far)
{
	_zFar = far;
}

float Camera::GetNear() const
{
	return _zNear;
}

float Camera::GetFar() const
{
	return _zFar;
}

ZObject* Camera::CreateInstance(string name, ObjectType type)
{
	return new Camera();
}

ZObject* Camera::Copy(string name, ZObject* object)
{
	if (object == nullptr || object->GetType() != CAMERA)
		return nullptr;

	auto source = static_cast<Camera*>(object);
	auto copy = new Camera();

	copy->SetClearColor(source->GetClearColor());
	copy->SetClearFlags(source->GetClearFlags());
	copy->SetNear(source->GetNear());
	copy->SetFar(source->GetFar());
	copy->SetFieldOfView(source->GetFieldOfView());
	copy->SetOrthoSize(source->GetOrthoSize());
	copy->SetProjectionMode(source->GetProjectionMode());
	copy->SetViewport(source->GetViewportX(), source->GetViewportY(), source->GetViewportWidth(), source->GetViewportHeight());
	copy->SetViewId(source->GetViewId());
	copy->SetRenderToTexture(source->IsRenderingToTexture());

	return copy;
}

Camera::~Camera()
{
	if (IsRenderingToTexture())
		bgfx::destroy(_frameBuffer);
}
