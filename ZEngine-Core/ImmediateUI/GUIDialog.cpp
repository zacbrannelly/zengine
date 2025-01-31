#include "GUIDialog.h"
#include "GUINameGenerator.h"

using namespace ZEngine;

GUIDialog::GUIDialog(std::string name, int width, int height, bool isModal) : GUIWindow(GUINameGenerator::GetUniqueName(name), width, height, false)
{
	_isVisible = false;
	_isModal = isModal;
	_result = DIALOG_RESULT_NONE;
}

void GUIDialog::Show()
{
	if (!ImGui::IsPopupOpen(GetTitle().c_str()))
	{
		ImGui::OpenPopup(GetTitle().c_str());
	}

	_isVisible = true;
}

void GUIDialog::Hide()
{
	ImGui::CloseCurrentPopup();

	_result = DIALOG_RESULT_NONE;
	_isVisible = false;
}

void GUIDialog::Close()
{
	if (IsVisible())
		ImGui::CloseCurrentPopup();

	SetCloseRequested(true);
}

void GUIDialog::RenderElement()
{
	// Show if requested
	if (_isVisible && _isModal)
	{
		Show();
	}

	ImGui::SetNextWindowSize(ImVec2(GetWidth(), GetHeight()), ImGuiCond_Appearing);

	bool open = true;
	bool* pOpen = IsCloseDisabled() ? nullptr : &open;
	bool begin = !_isModal
		? ImGui::BeginPopup(GetTitle().c_str(), GetFlags()) 
		: ImGui::BeginPopupModal(GetTitle().c_str(), pOpen, GetFlags());

	if (begin)
	{
		// Hide if requested
		if (!_isVisible) {
			Hide();
		}

		Container::RenderElement();
		RenderInWindow();
		ProcessInput();

		ImGui::EndPopup();
	}

	if (!open && AllowClose())
	{
		_isVisible = false;
		SetResult(DIALOG_RESULT_CLOSE);
	}
}

void GUIDialog::SetResult(GUIDialogResult result)
{
	_result = result;
}

GUIDialogResult GUIDialog::GetResult() const
{
	return _result;
}

void GUIDialog::SetModal(bool isModal)
{
	_isModal = isModal;
}

bool GUIDialog::IsModal() const
{
	return _isModal;
}

void GUIDialog::SetVisible(bool isVisible)
{
	_isVisible = isVisible;
}

bool GUIDialog::IsVisible() const
{
	return _isVisible;
}

void GUIDialog::SetDisableClose(bool disableClose)
{
	_disableClose = disableClose;
}

bool GUIDialog::IsCloseDisabled() const
{
	return _disableClose;
}

GUIDialog::~GUIDialog()
{

}