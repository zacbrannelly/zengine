#include "Editor.h"

#include <iostream>

#include <ZEngine-Core\Display\Display.h>
#include <ZEngine-Core\Rendering\Graphics.h>
#include <ZEngine-Core\Misc\Factory.h>
#include <ZEngine-Core\Map\Map.h>

#include "GUILibrary.h"
#include "MainMenuBar.h"
#include "MapView.h"
#include "GUIImage.h"

int main(int argc, char* argv[])
{
	// Initialize the factory (register the types)
	Factory::Init();

	// TODO: Register any editor specific ZObject's here

	// Init window
	Display display("ZEngine 3.0 | By Zac Brannelly", 1920, 1060);
	display.Init();

	// Init graphics sub-system
	auto graphics = Graphics::GetInstance();
	graphics->Init(&display);

	// Init GUI sub-system
	auto gui = GUILibrary::GetInstance();
	gui->Init(&display);

	// This container will hold all of the GUI elements
	Editor* editorContainer = new Editor();

	auto testMap = Factory::CreateInstance<Map>("test_map", ObjectType::MAP);

	while (!display.CloseRequested())
	{
		display.Update();

		// Render the GUI
		gui->NewFrame();
		editorContainer->RenderElement();
		gui->EndFrame();

		// Render the grey background
		graphics->Clear(0, 150, 150, 150, 255);
		graphics->Viewport(0, 0, 0, display.GetWidth(), display.GetHeight());
		graphics->Touch(0);

		// Head to the next frame (no draw calls beyond this point)
		graphics->Render();
	}

	// Clean up the GUI
	delete editorContainer;

	// Clean up
	gui->Shutdown();
	graphics->Shutdown();
	display.Shutdown();

	return 0;
}

Editor::Editor()
{
	Add(new MainMenuBar());
	Add(new MapView());
}

Editor::~Editor()
{
}