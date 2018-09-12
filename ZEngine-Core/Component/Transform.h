#pragma once

#include "Component.h"
#include <glm/glm.hpp>

class Transform : public Component
{
public:
	Transform();
	~Transform();

	void SetPosition(const glm::vec3& position);
	const glm::vec3& GetPosition() const;
	
	void SetScale(const glm::vec3& scale);
	const glm::vec3& GetScale() const;

	void SetRotation(const glm::vec3& rotation);
	const glm::vec3& GetRotation() const;

	glm::mat4 GetLocalTransformMatrix() const;
	glm::mat4 GetWorldTransformMatrix() const;

	void SetParent(Transform* parent);
	Transform* GetParent() const;

private:
	glm::vec3 _position;
	glm::vec3 _scale;
	glm::vec3 _rotation;

	Transform* _parent;

public:
	void Init() override;
	void Update() override;
	void Render(int viewId) override;

	static ZObject* CreateInstance(std::string name, ObjectType type);
};

