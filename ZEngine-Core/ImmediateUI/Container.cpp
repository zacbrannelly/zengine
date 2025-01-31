#include "Container.h"
#include "GUIWindow.h"

using namespace ZEngine;

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
			delete element;

			break;
		}
	}
}

void Container::Update()
{
	for (int i = 0; i < _elements.size(); ++i)
	{
		if (_elements[i] != nullptr)
		{
			auto element = _elements[i];
			element->Update();
		}
	}
}

void Container::RenderElement()
{
	std::vector<GUIElement*> killList;

	for (int i = 0; i < _elements.size(); ++i)
	{
		if (_elements[i] != nullptr)
		{
			auto element = _elements[i];

			element->RenderElement();

			// TODO: Find a more scalable way to check if it is a window
			if (element->GetType() == GUI_TYPE_WINDOW || element->GetType() == GUI_TYPE_CODE_EDITOR || element->GetType() == GUI_TYPE_SHADER_EDITOR || element->GetType() == GUI_TYPE_MATERIAL_EDITOR)
			{
				auto window = static_cast<GUIWindow*>(element);

				if (window->IsCloseRequested())
				{
					killList.push_back(window);
				}
			}
		}
	}

	for (auto element : killList)
		Remove(element);
}

const std::vector<GUIElement*>& Container::GetElements() const
{
	return _elements;
}

GUIElementType Container::GetType()
{
	return GUI_TYPE_CONTAINER;
}

Container::~Container()
{
	for (auto element : _elements)
	{
		if (element != nullptr)
			delete element;
	}
}
