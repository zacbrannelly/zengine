#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

Transform::Transform() : Component("Transform", ObjectType::TRANSFORM), _parent(nullptr)
{
}

void Transform::Init()
{

}

void Transform::Update()
{

}

void Transform::Render()
{

}

void Transform::SetPosition(const glm::vec3& position)
{
	_position = position;
}

const glm::vec3& Transform::GetPosition() const
{
	return _position;
}

void Transform::SetScale(const glm::vec3& scale)
{
	_scale = scale;
}

const glm::vec3& Transform::GetScale() const
{
	return _scale;
}

void Transform::SetRotation(const glm::vec3& rotation)
{
	_rotation = rotation;
}

const glm::vec3 & Transform::GetRotaion() const
{
	return _rotation;
}

glm::mat4 Transform::GetLocalTransformMatrix() const
{
	auto transform = glm::mat4(1.0f);

	transform = glm::translate(transform, _position);
	transform = glm::rotate(transform, _rotation.x, glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, _rotation.y, glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, _rotation.z, glm::vec3(0, 0, 1));
	transform = glm::scale(transform, _scale);

	return transform;
}

glm::mat4 Transform::GetWorldTransformMatrix() const
{
	if (_parent != nullptr)
	{
		return _parent->GetWorldTransformMatrix() * GetLocalTransformMatrix();
	}

	return GetLocalTransformMatrix();
}

void Transform::SetParent(Transform* parent)
{
	_parent = parent;
}

Transform* Transform::GetParent() const
{
	return _parent;
}

ZObject* Transform::CreateInstance(string name, ObjectType type)
{
	return new Transform();
}

Transform::~Transform()
{
}