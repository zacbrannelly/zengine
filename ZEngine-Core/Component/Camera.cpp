#include "Camera.h"
#include "../Rendering/Graphics.h"
#include "../Rendering/FrameBuffer.h"
#include "../Map/Objects/Entity.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace ZEngine;

Camera::Camera() : Component("Camera", ObjectType::CAMERA)
{
	RegisterDerivedType(CAMERA);
	_projection = ORTHOGRAPHIC;
	_frameBuffer = nullptr;
	_clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
	_zNear = 0.001f;
	_zFar = 100.0f;
	_viewport = { 0, 0, 1024, 600 };
	_clearColor = { 1, 1, 1, 1 };
	_orthoSize = 5.0f;
	_fieldOfView = glm::radians(60.0f);
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
	if (_frameBuffer != nullptr)
	{
		_graphics->SetFrameBuffer(_viewId, _frameBuffer);
	}
	_graphics->ViewMode(_viewId, bgfx::ViewMode::Default);
	_graphics->Viewport(_viewId, (int)(_viewport.x + 0.5f), (int)(_viewport.y + 0.5f), (int)(_viewport.z + 0.5f), (int)(_viewport.w + 0.5f));
	_graphics->Clear(_viewId, _clearFlags, (int)(_clearColor.r * 255.0f + 0.5f), (int)(_clearColor.g * 255.0f + 0.5f), (int)(_clearColor.b * 255.0f + 0.5f), (int)(_clearColor.a * 255.0f + 0.5f));

	// Calculate projection matrix based off projection mode
	auto projMatrix = GetProjectionMatrix();
	auto viewMatrix = GetViewMatrix();
	
	// Upload projection and view matrices to the GPU
	_graphics->ViewTransform(_viewId, projMatrix, viewMatrix);

	// Ensure the screen is cleared when nothing is being drawn (for testing purposes mostly, can remove later)
	_graphics->Touch(_viewId);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	auto aspectRatio = GetAspectRatio();

	if (_projection == Camera::ProjectionMode::ORTHOGRAPHIC)
	{
		// Create orthogonal projection using _ortho* fields + near & far.
		return glm::ortho<float>(_orthoLeft, _orthoRight, _orthoBottom, _orthoTop, _zNear, _zFar);
	}
	else
	{
		// Create perspective projection (3D) with the field of view (zoom)
		return glm::perspective<float>(_fieldOfView, aspectRatio, _zNear, _zFar);
	}
}

glm::mat4 Camera::GetViewMatrix() const
{
	// View matrix is the inverse of the camera's world transformation matrix
	return glm::inverse(GetOwner()->GetTransform()->GetWorldTransformMatrix());
}

glm::mat4 Camera::GetViewProjectionMatrix() const
{
	return GetProjectionMatrix() * GetViewMatrix();
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

void Camera::SetViewport(glm::vec4 viewport)
{
	SetViewport((int)viewport.x, (int)viewport.y, (int)viewport.z, (int)viewport.w);
}

void Camera::SetViewport(int x, int y, int width, int height)
{
	_viewport = { x, y, width, height };

	if (IsRenderingToTexture())
	{
		// Resize the frame buffer (this is a no-op if the frame buffer is not resizable)
		_frameBuffer->Resize(width, height);
	}
}

const glm::vec4& Camera::GetViewport() const
{
	return _viewport;
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
	_projection = mode;
}

Camera::ProjectionMode Camera::GetProjectionMode() const
{
	return _projection;
}

void Camera::SetFrameBuffer(FrameBuffer* frameBuffer)
{
	if (_frameBuffer != nullptr)
	{
		delete _frameBuffer;
	}
	_frameBuffer = frameBuffer;
}

FrameBuffer* Camera::GetFrameBuffer() const
{
	return _frameBuffer;
}

void Camera::SetRenderToTexture(bool renderToTexture)
{
	if (renderToTexture && !IsRenderingToTexture())
	{
		_frameBuffer = new FrameBuffer();
		_frameBuffer->Build(_viewport.z, _viewport.w);
	}
	else if (!renderToTexture && IsRenderingToTexture())
	{
		delete _frameBuffer;
		_frameBuffer = nullptr;
	}
}

bool Camera::IsRenderingToTexture() const
{
	return _frameBuffer != nullptr;
}

bgfx::TextureHandle Camera::GetRenderTexture() const
{
	return _frameBuffer != nullptr 
		? _frameBuffer->GetTexture() 
		: bgfx::TextureHandle { bgfx::kInvalidHandle };
}

void Camera::SetFieldOfView(float fov)
{
	_fieldOfView = glm::radians(fov);
}

float Camera::GetFieldOfView() const
{
	return glm::degrees(_fieldOfView);
}

void Camera::SetOrthoSize(float size)
{
	_orthoSize = size;
	
	auto aspectRatio = GetAspectRatio();
	_orthoLeft = -aspectRatio * _orthoSize;
	_orthoRight = aspectRatio * _orthoSize;
	_orthoBottom = -_orthoSize;
	_orthoTop = _orthoSize;
}

float Camera::GetOrthoSize() const
{
	return _orthoSize;
}

void Camera::SetOrtho(float left, float right, float bottom, float top)
{
	_orthoLeft = left;
	_orthoRight = right;
	_orthoBottom = bottom;
	_orthoTop = top;
}

void Camera::GetOrtho(float& left, float& right, float& bottom, float& top) const
{
	left = _orthoLeft;
	right = _orthoRight;
	bottom = _orthoBottom;
	top = _orthoTop;
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
	if (_frameBuffer != nullptr) {
		delete _frameBuffer;
	}
}
