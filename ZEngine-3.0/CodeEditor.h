#pragma once

#include "GUIWindow.h"

class UnsavedDialog;
class File;

class CodeEditor : public GUIWindow
{
public:
	CodeEditor(std::string path);
	~CodeEditor();

	void ProcessInput() override;
	void RenderInWindow() override;
	GUIElementType GetType() override;
	bool AllowClose() override;

	const File& GetFile();

	void SetDirty(bool dirty);
	bool IsDirty() const;

	void Save();

private:
	static int TextInputCallback(ImGuiInputTextCallbackData* data);

	UnsavedDialog* _unsavedDialog;
	bool _showUnsavedDialog;
	char* _originalBuffer;
	char* _buffer;
	int _bufferSize;
	File* _file;
};

