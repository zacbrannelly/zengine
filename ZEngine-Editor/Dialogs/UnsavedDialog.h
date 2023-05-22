#pragma once

#include "../UI/GUIWindow.h"

class UnsavedDialog : public GUIElement
{
public:
	UnsavedDialog();
	~UnsavedDialog();

	void RenderElement() override;
	GUIElementType GetType() override;

	void Reset();
	bool ShouldClose() const;
	bool ShouldSave() const;
	bool ShouldNotSave() const;

private:
	bool _shouldClose;
	bool _shouldSave;
	bool _shouldNotSave;
};