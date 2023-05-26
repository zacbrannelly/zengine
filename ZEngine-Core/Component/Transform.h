#pragma once

#include <vector>
#include "Component.h"
#include <glm/glm.hpp>

class Transform : public Component
{
public:
	Transform();
	~Transform();

	void SetPosition(const glm::vec3& position);
	const glm::vec3& GetPosition() const;

	void SetPosition(float x, float y, float z);
	float GetPositionX() const;
	float GetPositionY() const;
	float GetPositionZ() const;
	
	void SetScale(const glm::vec3& scale);
	const glm::vec3& GetScale() const;

	void SetRotation(const glm::vec3& rotation);
	const glm::vec3& GetRotation() const;

	const glm::mat4 GetLocalTransformMatrix() const;
	const glm::mat4 GetWorldTransformMatrix() const;

	void SetParent(Transform* parent);
	Transform* GetParent() const;

	const std::vector<Transform*>& GetChildren() const;

private:
	glm::vec3 _position;
	glm::vec3 _scale;
	glm::vec3 _rotation;

	Transform* _parent;
	std::vector<Transform*> _children;

public:
	void Init() override;
	void Update() override;
	void Render(int viewId) override;

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

#ifndef SWIGCSHARP
	static v8::Global<v8::FunctionTemplate> GetTemplate(v8::Isolate* isolate);
#endif
};

