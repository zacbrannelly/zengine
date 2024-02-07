#pragma once

#include "DynamicCollider3D.h"
#include "../Utilities/JsonHelpers.h"

#include <glm/glm.hpp>

class BoxCollider3D : public DynamicCollider3D
{
public:
	BoxCollider3D();
	~BoxCollider3D();

	void SetSize(glm::vec3 size);
	glm::vec3 GetSize() const;
	glm::vec3 GetWorldSize() const;

	virtual void BuildGeometry() override;

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

	static ObjectType GetStaticType()
	{
		return BOX_COLLIDER_3D;
	}

#ifndef SWIG
	// Allow serialization / deserialization
	JSON_SCHEMA_BEGIN(BoxCollider3D)
		JSON_MAP_TO_SETTER_OPTIONAL (isTrigger, SetIsTrigger, bool)
		JSON_MAP_TO_SETTER_OPTIONAL (size,      SetSize,      glm::vec3)
	JSON_SCHEMA_END()
#endif

private:
	glm::vec3 _size;
};
