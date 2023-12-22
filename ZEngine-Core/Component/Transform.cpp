#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

Transform::Transform() : Component("Transform", ObjectType::TRANSFORM)
{
	RegisterDerivedType(TRANSFORM);
	_position = glm::vec3(0);
	_rotation = glm::quat(1.0f, 0, 0, 0);
	_scale = glm::vec3(1.0f);
	_rotation = glm::vec3(0);
	_parent = nullptr;
}

void Transform::Init()
{
}

void Transform::Update()
{
}

void Transform::Render(int viewId)
{
}

void Transform::Set(Transform* source)
{
    _position = source->GetPosition();
    _rotation = source->GetRotationQuaternion();
    _scale = source->GetScale();
}

void Transform::SetPosition(const glm::vec3& position)
{
	_position = position;
}

void Transform::SetPosition(float x, float y, float z)
{
	_position = glm::vec3(x, y, z);
}

const glm::vec3& Transform::GetPosition() const
{
	return _position;
}

float Transform::GetPositionX() const
{
	return _position.x;
}

float Transform::GetPositionY() const
{
	return _position.y;
}

float Transform::GetPositionZ() const
{
	return _position.z;
}

void Transform::SetScale(const glm::vec3& scale)
{
	_scale = scale;
}

const glm::vec3& Transform::GetScale() const
{
	return _scale;
}

void Transform::Rotate(const glm::vec3& rotation)
{
	_rotation = glm::quat(glm::radians(rotation)) * _rotation;
}

void Transform::SetRotation(const glm::vec3& rotation)
{
	_rotation = glm::quat(glm::radians(rotation));
}

glm::vec3 Transform::GetRotation() const
{
	return glm::degrees(glm::eulerAngles(_rotation));
}

void Transform::SetRotationQuaternion(const glm::quat& rotation)
{
	_rotation = rotation;
}

const glm::quat& Transform::GetRotationQuaternion() const
{
	return _rotation;
}

const glm::mat4 Transform::GetLocalTransformMatrix() const
{
	auto transform = glm::mat4(1.0f);

	transform = glm::translate(transform, _position);
	transform = transform * glm::toMat4(_rotation);
	transform = glm::scale(transform, _scale);

	return transform;
}

const glm::mat4 Transform::GetWorldTransformMatrix() const
{
	if (_parent != nullptr)
	{
		return _parent->GetWorldTransformMatrix() * GetLocalTransformMatrix();
	}

	return GetLocalTransformMatrix();
}

void Transform::SetParent(Transform* parent)
{
	if (_parent != nullptr)
	{
		auto it = std::find(_parent->GetChildren().begin(), _parent->GetChildren().end(), this);

		if (it != _parent->GetChildren().end())
		{
			_parent->_children.erase(it);
		}
	}

	_parent = parent;

	if (_parent != nullptr)
	{
		_parent->_children.push_back(this);
	}
}

Transform* Transform::GetParent() const
{
	return _parent;
}

const std::vector<Transform*>& Transform::GetChildren() const
{
	return _children;
}

ZObject* Transform::CreateInstance(string name, ObjectType type)
{
	return new Transform();
}

ZObject* Transform::Copy(string name, ZObject* object)
{
	if (object == nullptr || object->GetType() != TRANSFORM)
		return nullptr;

	auto source = static_cast<Transform*>(object);
	auto copy = new Transform();

	copy->SetPosition(source->GetPosition());
	copy->SetRotation(source->GetRotation());
	copy->SetScale(source->GetScale());

	return copy;
}

Transform::~Transform()
{
}
