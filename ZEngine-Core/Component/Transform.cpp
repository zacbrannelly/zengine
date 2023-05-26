#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Scripting/ScriptSystem.h"
#include "../Scripting/Wrappers/Vec3Wrapper.h"

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

void Transform::SetRotation(const glm::vec3& rotation)
{
	_rotation = rotation;
}

const glm::vec3 & Transform::GetRotation() const
{
	return _rotation;
}

const glm::mat4 Transform::GetLocalTransformMatrix() const
{
	auto transform = glm::mat4(1.0f);

	transform = glm::translate(transform, _position);
	transform = glm::rotate(transform, glm::radians(_rotation.x), glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, glm::radians(_rotation.y), glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, glm::radians(_rotation.z), glm::vec3(0, 0, 1));
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

void Transform_GetPositionCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto self = info.Holder();
	auto wrapper = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	auto scriptableObj = static_cast<IScriptable*>(wrapper->Value());
	auto transform = static_cast<Transform*>(scriptableObj);
	auto context = info.GetIsolate()->GetCurrentContext();

	if (info.Length() == 0)
	{
		// TODO: Try not to create a new instance every time, perhaps make one instance and save as private field..
		auto wrapVec3 = Vec3Wrapper::NewInstance();
		wrapVec3->SetData(transform->GetPosition());

		info.GetReturnValue().Set(wrapVec3->GetObject());
	}
	else if (info.Length() == 1)
	{
		wrapper = v8::Local<v8::External>::Cast(info[0]->ToObject(context).ToLocalChecked()->GetInternalField(0));
		auto wrapVec3 = static_cast<Vec3Wrapper*>(wrapper->Value());
		wrapVec3->SetData(transform->GetPosition());

		info.GetReturnValue().Set(wrapVec3->GetObject());
	}
}

void Transform_GetRotationCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto self = info.Holder();
	auto wrapper = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	auto scriptableObj = static_cast<IScriptable*>(wrapper->Value());
	auto transform = static_cast<Transform*>(scriptableObj);
	auto context = info.GetIsolate()->GetCurrentContext();

	if (info.Length() == 0)
	{
		// TODO: Try not to create a new instance every time, perhaps make one instance and save as private field..
		auto wrapVec3 = Vec3Wrapper::NewInstance();
		wrapVec3->SetData(transform->GetRotation());

		info.GetReturnValue().Set(wrapVec3->GetObject());
	}
	else if (info.Length() == 1)
	{
		wrapper = v8::Local<v8::External>::Cast(info[0]->ToObject(context).ToLocalChecked()->GetInternalField(0));
		auto wrapVec3 = static_cast<Vec3Wrapper*>(wrapper->Value());
		wrapVec3->SetData(transform->GetPosition());

		info.GetReturnValue().Set(wrapVec3->GetObject());
	}
}

void Transform_GetScaleCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto self = info.Holder();
	auto wrapper = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	auto scriptableObj = static_cast<IScriptable*>(wrapper->Value());
	auto transform = static_cast<Transform*>(scriptableObj);
	auto context = info.GetIsolate()->GetCurrentContext();

	if (info.Length() == 0)
	{
		// TODO: Try not to create a new instance every time, perhaps make one instance and save as private field..
		auto wrapVec3 = Vec3Wrapper::NewInstance();
		wrapVec3->SetData(transform->GetScale());

		info.GetReturnValue().Set(wrapVec3->GetObject());
	}
	else if (info.Length() == 1)
	{
		wrapper = v8::Local<v8::External>::Cast(info[0]->ToObject(context).ToLocalChecked()->GetInternalField(0));
		auto wrapVec3 = static_cast<Vec3Wrapper*>(wrapper->Value());
		wrapVec3->SetData(transform->GetPosition());

		info.GetReturnValue().Set(wrapVec3->GetObject());
	}
}

void Transform_Getter(v8::Local<v8::String> nameObj, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	auto sys = ScriptSystem::GetInstance();
	auto name = sys->CastString(nameObj);
	
	auto wrap = v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0));
	auto scriptable = static_cast<IScriptable*>(wrap->Value());
	auto transform = static_cast<Transform*>(scriptable);

	if (transform == nullptr)
		return;

	if (name == "position")
	{
		auto vecWrapper = Vec3Wrapper::NewInstance();
		vecWrapper->SetData(transform->GetPosition());

		info.GetReturnValue().Set(vecWrapper->GetObject());
	}
	else if (name == "rotation")
	{
		auto vecWrapper = Vec3Wrapper::NewInstance();
		vecWrapper->SetData(transform->GetRotation());

		info.GetReturnValue().Set(vecWrapper->GetObject());
	}
	else if (name == "scale")
	{
		auto vecWrapper = Vec3Wrapper::NewInstance();
		vecWrapper->SetData(transform->GetScale());

		info.GetReturnValue().Set(vecWrapper->GetObject());
	}
	else if (name == "worldPosition")
	{
		auto vecWrapper = Vec3Wrapper::NewInstance();
		vecWrapper->SetData(transform->GetWorldTransformMatrix() * glm::vec4(transform->GetPosition(), 1));

		info.GetReturnValue().Set(vecWrapper->GetObject());
	}
}

void Transform_Setter(v8::Local<v8::String> nameObj, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	auto sys = ScriptSystem::GetInstance();
	auto name = sys->CastString(nameObj);

	auto wrap = v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0));
	auto scriptable = static_cast<IScriptable*>(wrap->Value());
	auto transform = static_cast<Transform*>(scriptable);
	auto context = info.GetIsolate()->GetCurrentContext();


	if (transform == nullptr)
		return;

	if (name == "position")
	{
		wrap = v8::Local<v8::External>::Cast(value->ToObject(context).ToLocalChecked()->GetInternalField(0));
		auto vecWrapper = static_cast<Vec3Wrapper*>(wrap->Value());
		
		transform->SetPosition(vecWrapper->GetData());
	}
	else if (name == "rotation")
	{
		wrap = v8::Local<v8::External>::Cast(value->ToObject(context).ToLocalChecked()->GetInternalField(0));
		auto vecWrapper = static_cast<Vec3Wrapper*>(wrap->Value());

		transform->SetRotation(vecWrapper->GetData());
	}
	else if (name == "scale")
	{
		wrap = v8::Local<v8::External>::Cast(value->ToObject(context).ToLocalChecked()->GetInternalField(0));
		auto vecWrapper = static_cast<Vec3Wrapper*>(wrap->Value());

		transform->SetScale(vecWrapper->GetData());
	}
}

v8::Global<v8::FunctionTemplate> Transform::GetTemplate(v8::Isolate* isolate)
{
	using namespace v8;

	auto sys = ScriptSystem::GetInstance();

	auto constructor = FunctionTemplate::New(isolate);
	constructor->SetClassName(sys->GetString("Transform"));

	constructor->InstanceTemplate()->SetInternalFieldCount(1);

	//Add functions 
	constructor->InstanceTemplate()->Set(isolate, "SetPosition", FunctionTemplate::New(isolate, Transform_SetPositionCallback));
	constructor->InstanceTemplate()->Set(isolate, "GetPosition", FunctionTemplate::New(isolate, Transform_GetPositionCallback));
	constructor->InstanceTemplate()->Set(isolate, "SetRotation", FunctionTemplate::New(isolate, Transform_SetRotationCallback));
	constructor->InstanceTemplate()->Set(isolate, "GetRotation", FunctionTemplate::New(isolate, Transform_GetRotationCallback));
	constructor->InstanceTemplate()->Set(isolate, "SetScale", FunctionTemplate::New(isolate, Transform_SetScaleCallback));
	constructor->InstanceTemplate()->Set(isolate, "GetScale", FunctionTemplate::New(isolate, Transform_GetScaleCallback));

	// Add properties
	constructor->InstanceTemplate()->SetAccessor(sys->GetString("position"), Transform_Getter, Transform_Setter);
	constructor->InstanceTemplate()->SetAccessor(sys->GetString("rotation"), Transform_Getter, Transform_Setter);
	constructor->InstanceTemplate()->SetAccessor(sys->GetString("scale"), Transform_Getter, Transform_Setter);
	constructor->InstanceTemplate()->SetAccessor(sys->GetString("worldPosition"), Transform_Getter, Transform_Setter);

	return Global<FunctionTemplate>(isolate, constructor);
}

Transform::~Transform()
{
}