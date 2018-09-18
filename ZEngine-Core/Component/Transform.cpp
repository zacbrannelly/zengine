#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Scripting/ScriptSystem.h"

using namespace std;

Transform::Transform() : Component("Transform", ObjectType::TRANSFORM)
{
	_position = glm::vec3(0);
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

const glm::vec3 & Transform::GetRotation() const
{
	return _rotation;
}

glm::mat4 Transform::GetLocalTransformMatrix() const
{
	auto transform = glm::mat4(1.0f);

	transform = glm::translate(transform, _position);
	transform = glm::rotate(transform, glm::radians(_rotation.x), glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, glm::radians(_rotation.y), glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, glm::radians(_rotation.z), glm::vec3(0, 0, 1));
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

void Transform_SetPositionCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	if (info.Length() == 3)
	{
		auto self = info.Holder();
		auto wrapper = v8::Local<v8::External>::Cast(self->GetInternalField(0));
		auto scriptableObj = static_cast<IScriptable*>(wrapper->Value());
		auto transform = static_cast<Transform*>(scriptableObj);
		auto context = ScriptSystem::GetInstance()->GetContext()->GetLocal();

		double x = info[0]->NumberValue(context).ToChecked();
		double y = info[1]->NumberValue(context).ToChecked();
		double z = info[2]->NumberValue(context).ToChecked();

		transform->SetPosition({ x, y, z });
	}
}

void Transform_SetRotationCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	if (info.Length() == 3)
	{
		auto self = info.Holder();
		auto wrapper = v8::Local<v8::External>::Cast(self->GetInternalField(0));
		auto scriptableObj = static_cast<IScriptable*>(wrapper->Value());
		auto transform = static_cast<Transform*>(scriptableObj);
		auto context = ScriptSystem::GetInstance()->GetContext()->GetLocal();

		double x = info[0]->NumberValue(context).ToChecked();
		double y = info[1]->NumberValue(context).ToChecked();
		double z = info[2]->NumberValue(context).ToChecked();

		transform->SetRotation({ x, y, z });
	}
}

void Transform_SetScaleCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	if (info.Length() == 3)
	{
		auto self = info.Holder();
		auto wrapper = v8::Local<v8::External>::Cast(self->GetInternalField(0));
		auto scriptableObj = static_cast<IScriptable*>(wrapper->Value());
		auto transform = static_cast<Transform*>(scriptableObj);
		auto context = ScriptSystem::GetInstance()->GetContext()->GetLocal();

		double x = info[0]->NumberValue(context).ToChecked();
		double y = info[1]->NumberValue(context).ToChecked();
		double z = info[2]->NumberValue(context).ToChecked();

		transform->SetScale({ x, y, z });
	}
}

v8::Global<v8::FunctionTemplate> Transform::GetTemplate(v8::Isolate* isolate)
{
	using namespace v8;

	auto sys = ScriptSystem::GetInstance();

	auto constructor = FunctionTemplate::New(isolate);
	constructor->SetClassName(sys->GetString("Transform"));

	constructor->InstanceTemplate()->SetInternalFieldCount(1);

	//TODO: Add properties (getters/setters only)
	constructor->InstanceTemplate()->Set(isolate, "SetPosition", FunctionTemplate::New(isolate, Transform_SetPositionCallback));
	constructor->InstanceTemplate()->Set(isolate, "SetRotation", FunctionTemplate::New(isolate, Transform_SetRotationCallback));
	constructor->InstanceTemplate()->Set(isolate, "SetScale", FunctionTemplate::New(isolate, Transform_SetScaleCallback));

	return Global<FunctionTemplate>(isolate, constructor);
}

Transform::~Transform()
{
}