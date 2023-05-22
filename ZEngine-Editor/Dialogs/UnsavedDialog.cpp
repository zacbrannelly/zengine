#include "UnsavedDialog.h"

#include <iostream>

UnsavedDialog::UnsavedDialog()
{
	_shouldClose = false;
	_shouldSave = false;
	_shouldNotSave = false;
}

void UnsavedDialog::RenderElement()
{
	if (!ImGui::IsPopupOpen("Unsaved changes detected!"))
		ImGui::OpenPopup("Unsaved changes detected!");

	bool open = true;

	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Appearing);
	if (ImGui::BeginPopupModal("Unsaved changes detected!", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Text("Would you like to save your changes?");

		if (ImGui::Button("Yes", ImVec2(50, 30)))
		{
			_shouldSave = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(50, 30)))
		{
			_shouldNotSave = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(50, 30)))
		{
			_shouldClose = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (!open)
	{
		_shouldClose = true;
		ImGui::CloseCurrentPopup();
	}
}

void UnsavedDialog::Reset()
{
	_shouldClose = false;
	_shouldSave = false;
	_shouldNotSave = false;
}

bool UnsavedDialog::ShouldClose() const
{
	return _shouldClose;
}

bool UnsavedDialog::ShouldSave() const
{
	return _shouldSave;
}

bool UnsavedDialog::ShouldNotSave() const
{
	return _shouldNotSave;
}

GUIElementType UnsavedDialog::GetType()
{
	return GUI_TYPE_ELEMENT;
}

UnsavedDialog::~UnsavedDialog()
{

}