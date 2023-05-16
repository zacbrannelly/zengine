#pragma once

#include <string>
#include "../../Scripting/IScriptable.h"
#include "ObjectType.h"

class ZObject : public IScriptable
{
public:
	ZObject(std::string name, ObjectType type);
	virtual ~ZObject();

	ObjectType GetType() const;
	std::string GetName() const;

	static ZObject* CreateInstance(std::string name, ObjectType type);
private:
	std::string _name;
	ObjectType _type;
};

