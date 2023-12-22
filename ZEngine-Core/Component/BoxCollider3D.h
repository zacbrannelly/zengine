#pragma once

#include "DynamicCollider3D.h"
#include <glm/glm.hpp>

class Mesh;
class Material;
class MeshRenderer;
class Transform;

class BoxCollider3D : public DynamicCollider3D
{
public:
	BoxCollider3D();
	~BoxCollider3D();

	void SetSize(glm::vec3 size);
	glm::vec3 GetSize() const;

    virtual void RenderGizmos(int viewId) override;

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

	static ObjectType GetStaticType()
	{
		return BOX_COLLIDER_3D;
	}

private:
	glm::vec3 _size;
    MeshRenderer* _cubeGizmoRenderer;
    Transform* _cubeGizmoTransform;
};
