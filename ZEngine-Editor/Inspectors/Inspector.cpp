#include "Inspector.h"
#include <sstream>

using namespace ZEngine;

Inspector::Inspector()
{
	_value = nullptr;
}

void Inspector::Inspect(Component* component)
{
	_value = component;
}

Component* Inspector::GetValue() const
{
	return _value;
}

std::string Inspector::UniqueLabel(std::string label) const
{
	std::stringstream ss;
	ss << (void*)_value;
	auto uniqueId = ss.str();
	return label + "###" + uniqueId;
}

GUIElementType Inspector::GetType()
{
	return GUI_TYPE_INSPECTOR;
}

Inspector::~Inspector()
{
}
