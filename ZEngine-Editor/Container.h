#pragma once
#include <vector>
#include "GUIElement.h"

class Container : public GUIElement
{
public:
	Container();
	virtual ~Container();

	virtual void Add(GUIElement* element);
	virtual void Remove(GUIElement* element);

	const std::vector<GUIElement*>& GetElements() const;

	virtual void RenderElement() override;
	virtual GUIElementType GetType() override;

private:
	std::vector<GUIElement*> _elements;
};

