#pragma once

#include <vector>
#include "Container.h"

class GUIWindow : public Container
{
public:
	GUIWindow(std::string title, int width, int height, bool isChild);
	virtual ~GUIWindow();

	virtual void ProcessInput() = 0;
	virtual void RenderInWindow() = 0;
	virtual void RenderElement() override;

	virtual void SetSize(int width, int height);
	virtual int GetWidth() const;
	virtual int GetHeight() const;
private:
	std::string _title;
	int _width, _height;
	bool _isChild;
	bool _shouldSetSize;
};

