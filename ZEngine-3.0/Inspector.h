#pragma once

#include "GUIElement.h"
#include <ZEngine-Core/Component/Component.h>

class Inspector : public GUIElement
{
public:
	Inspector();
	~Inspector();

	virtual void Inspect(Component* component);
	Component* GetValue() const;

	virtual void RenderElement() = 0;

private:
	Component* _value;
};

