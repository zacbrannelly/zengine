#include "Container.h"


Container::Container()
{
}

void Container::Add(GUIElement* element)
{
	_elements.push_back(element);
}

void Container::Remove(GUIElement* element)
{
	for (auto it = _elements.begin(); it != _elements.end(); it++)
	{
		if (*it == element)
		{
			_elements.erase(it);
			break;
		}
	}
}

void Container::RenderElement()
{
	for (auto element : _elements)
	{
		if (element != nullptr)
			element->RenderElement();
	}
}

const std::vector<GUIElement*>& Container::GetElements() const
{
	return _elements;
}

Container::~Container()
{
	for (auto element : _elements)
	{
		if (element != nullptr)
			delete element;
	}
}
