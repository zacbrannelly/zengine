#pragma once

#include "GUIWindow.h"

enum GUIDialogResult
{
	DIALOG_RESULT_NONE,
	DIALOG_RESULT_OK,
	DIALOG_RESULT_NO,
	DIALOG_RESULT_CLOSE
};

class GUIDialog : public GUIWindow
{
public:
	GUIDialog(std::string title, int width, int height, bool modal);
	virtual ~GUIDialog();

	void SetModal(bool isModal);
	bool IsModal() const;

	void SetVisible(bool isVisible);
	bool IsVisible() const;

	virtual void Show();
	virtual void Hide();
	virtual void Close();

	virtual void RenderElement() override;

	virtual void SetResult(GUIDialogResult result);
	virtual GUIDialogResult GetResult() const;

private:
	bool _isModal;
	bool _isVisible;
	GUIDialogResult _result;
};