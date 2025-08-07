#include "ImportDialog.h"
#include "../Editor.h"
#include "../Project/Project.h"
#include <ZEngine-Core/Assets/AssetManager.h>

using namespace ZEngine;

ImportDialog::ImportDialog(Editor* editor, Directory targetDirectory) : BrowserDialog(), _editor(editor), _targetDirectory(targetDirectory)
{
  SetTitle("Import...");
  SetModal(true);
  SetVisible(true);
}

void ImportDialog::RenderInWindow()
{
  BrowserDialog::RenderInWindow();

  if (IsVisible())
  {
    auto result = GetResult();
    if (result == DIALOG_RESULT_CLOSE)
    {
      Close();
    }
    else if (result == DIALOG_RESULT_OK)
    {
      const auto file = GetFile();
      const auto path = file.GetPath();

      ObjectType type;
      if (GetObjectTypeFromExtension(file.GetExtension(), type)) {
        // TODO: Detect files that require asset files (e.g. .cs, .wav, .mp3, etc.)

        // Copy the file to the target directory
        const auto targetPath = _targetDirectory.GetPath() + file.GetFilename();
        file.Copy(targetPath);

        // Import the copy into the project
        _editor->GetProject()->GetCatalog().RegisterAsset(targetPath, type);
        _editor->GetProject()->Save();

        // Close the dialog
        Close();
      }
    }
  }
}

ImportDialog::~ImportDialog()
{
}
