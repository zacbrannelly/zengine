#pragma once

#include <vector>
#include "Container.h"

typedef int ImGuiWindowFlags;

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

	void SetFlags(ImGuiWindowFlags flags);
	ImGuiWindowFlags GetFlags() const;
private:
	std::string _title;
	int _width, _height;
	bool _isChild;
	bool _shouldSetSize;
	ImGuiWindowFlags _flags;
};

