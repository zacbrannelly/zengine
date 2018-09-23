#include "InputTypes.h"
#include "../Scripting/ScriptSystem.h"

void InstallButtonCodesIntoScripting(ScriptSystem* system)
{
	// Get the global namespace obj
	auto global = system->GetContext()->GetLocal()->Global();

	global->Set(system->GetString("BUTTON_KEY_LEFT"), v8::Int32::New(system->GetIsolate(), BUTTON_KEY_LEFT));
	global->Set(system->GetString("BUTTON_KEY_RIGHT"), v8::Int32::New(system->GetIsolate(), BUTTON_KEY_RIGHT));
	global->Set(system->GetString("BUTTON_KEY_UP"), v8::Int32::New(system->GetIsolate(), BUTTON_KEY_UP));
	global->Set(system->GetString("BUTTON_KEY_DOWN"), v8::Int32::New(system->GetIsolate(), BUTTON_KEY_DOWN));
	
	// TODO: Do the rest of the button code bindings
}