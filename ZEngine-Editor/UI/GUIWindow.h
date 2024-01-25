#pragma once

#include <string>
#include <vector>
#include "Container.h"
#include "../imgui-includes.h"

class GUIWindow : public Container
{
public:
	GUIWindow(std::string title, int width, int height, bool isChild);
	virtual ~GUIWindow();

	virtual void ProcessInput() = 0;
	virtual void RenderInWindowBeforeElements();
	virtual void RenderInWindow() = 0;
	virtual void RenderElement() override;
	virtual GUIElementType GetType() override;

	virtual void SetSize(int width, int height);
	virtual int GetWidth() const;
	virtual int GetHeight() const;

	void SetUserData(void* data);
	void* GetUserData() const;

	void Focus();

	void SetTitle(std::string title);
	const std::string& GetTitle() const;

	const ImVec2& GetContentSize() const;
	int GetContentWidth() const;
	int GetContentHeight() const;

	void SetFlags(ImGuiWindowFlags flags);
	ImGuiWindowFlags GetFlags() const;

	void SetDirty(bool dirty);
	bool IsDirty() const;

	virtual bool AllowClose();
	void SetCloseRequested(bool closeRequested);
	bool IsCloseRequested() const;

private:
	std::string _title;
	int _width, _height;
	ImVec2 _contentSize;
	bool _isChild;
	bool _shouldSetSize;
	bool _shouldFocus;
	bool _closeRequested;
	bool _isDirty;
	ImGuiWindowFlags _flags;
	void* _data;
};

