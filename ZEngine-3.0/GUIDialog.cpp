#include "GUIDialog.h"
#include "GUINameGenerator.h"

GUIDialog::GUIDialog(std::string name, int width, int height, bool isModal) : GUIWindow(GUINameGenerator::GetUniqueName(name), width, height, false)
{
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
	if (ImGui::IsPopupOpen(GetTitle().c_str()))
	{
		ImGui::CloseCurrentPopup();
	}

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
	auto visible = ImGui::IsPopupOpen(GetTitle().c_str());

	if (visible && !_isVisible)
	{
		Hide();
	}
	else if (_isVisible && !visible)
	{
		Show();
	}

	ImGui::SetNextWindowSize(ImVec2(GetWidth(), GetHeight()), ImGuiCond_Appearing);

	bool open = true;
	bool begin = !_isModal ? ImGui::BeginPopup(GetTitle().c_str(), GetFlags()) : ImGui::BeginPopupModal(GetTitle().c_str(), &open, GetFlags());

	if (begin)
	{
		Container::RenderElement();
		RenderInWindow();
		ProcessInput();

		ImGui::EndPopup();
	}

	if (!open && AllowClose())
	{
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

bool GUIDialog::IsVisible() const
{
	return _isVisible;
}

GUIDialog::~GUIDialog()
{

}