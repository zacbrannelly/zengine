#include "CodeEditor.h"
#include "../Dialogs/UnsavedDialog.h"
#include "../imgui-includes.h"
#include <ZEngine-Core/Utilities/File.h>

#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Input/InputManager.h>
#include <iostream>

using namespace std;

CodeEditor::CodeEditor(string path) : GUIWindow("Code Editor", 600, 500, false)
{
	_showUnsavedDialog = false;

	_file = new File(path);
	SetTitle(_file->GetFilename() + " - Code Editor");

	// Create a buffer for the working copy of the file
	_buffer = new char[1000000];
	_bufferSize = 1000000;
	memset(_buffer, '\0', _bufferSize);

	// Fill the buffer with the contents of the file
	_file->ReadContent(_buffer, _bufferSize);

	// Keep a copy of the contents for change detection
	_originalBuffer = new char[_bufferSize];
	memcpy(_originalBuffer, _buffer, _bufferSize);

	SetFlags(ImGuiWindowFlags_MenuBar);

	_unsavedDialog = new UnsavedDialog();
}

void CodeEditor::Save()
{
	if (_file->WriteContent(_buffer, strlen(_buffer)))
	{
		// Set the original to the new saved data
		memcpy(_originalBuffer, _buffer, _bufferSize);

		// Ensure the doc isn't considered dirty anymore
		SetDirty(false);

		// Hot reload the script so changes are reflected when playing in editor
		HotReload();
	}
}

void CodeEditor::HotReload()
{
	// TODO: Implement C# hot reload.
}

bool CodeEditor::AllowClose()
{
	if (IsDirty())
	{
		_showUnsavedDialog = true;
		return false;
	}
	else
		return true;
}

void CodeEditor::ProcessInput()
{
}

void CodeEditor::RenderInWindow()
{
	// Detect shortcuts in here
	if (ImGui::IsWindowFocused())
	{
		auto input = InputManager::GetInstance();

		// Save shortcut
		if (input->GetButtonPressed(BUTTON_KEY_S) && input->ButtonHasModifier(BUTTON_KEY_S, BUTTON_MOD_CONTROL))
		{
			Save();
		}
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save & Build", "Ctrl+S", false))
			{
				Save();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	auto font = ImGui::GetIO().Fonts[0].Fonts[1];
	ImGui::PushFont(font);
	ImGui::InputTextMultiline((string("#code") + _file->GetFilename()).c_str(), _buffer, _bufferSize, GetContentSize(), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CallbackAlways, TextInputCallback, this);
	ImGui::PopFont();

	if (_showUnsavedDialog)
	{
		_unsavedDialog->RenderElement();

		if (_unsavedDialog->ShouldClose())
		{
			_showUnsavedDialog = false;
			_unsavedDialog->Reset();
		}
		else if (_unsavedDialog->ShouldSave())
		{
			Save();
			SetCloseRequested(true);
		}
		else if (_unsavedDialog->ShouldNotSave())
		{
			SetCloseRequested(true);
		}
	}
}

const File& CodeEditor::GetFile()
{
	return *_file;
}

int CodeEditor::TextInputCallback(ImGuiInputTextCallbackData* data)
{
	// Check whether changes have been made since last save
	auto editor = static_cast<CodeEditor*>(data->UserData);
	editor->SetDirty(strcmp(editor->_buffer, editor->_originalBuffer) != 0);

	return 0;
}

GUIElementType CodeEditor::GetType()
{
	return GUI_TYPE_CODE_EDITOR;
}

CodeEditor::~CodeEditor()
{
	delete _file;
	delete[] _buffer;
}
