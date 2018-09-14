#include "Inspector.h"



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

Inspector::~Inspector()
{
}
