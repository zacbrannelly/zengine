#include "MapController.h"

#include <ZEngine-Core/Audio/AudioSystem.h>
#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Utilities/FutureHelpers.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/MapManager.h>

#include "../Editor.h"
#include "../Project/Project.h"

MapController::MapController(Editor* editor) : _editor(editor), _originalMap(nullptr), _previewMap(nullptr), _playState(STOPPED)
{
}

void MapController::Update()
{
	// Update the map's game state if we're playing and we're not in the process of changing the map.
	if (_playState == PLAYING && _updateMapLock.try_lock())
	{
		_editor->GetSelectedMap()->Update();
		_updateMapLock.unlock();
	}
}

void MapController::Play()
{
	if (_playState == PLAYING || _editor->GetSelectedMap() == nullptr)
		return;

	if (_playState == PAUSED) {
		Continue();
		return;
	}

	// Build the project scripts and then start playing.
	_buildFuture = _editor->GetProject()->BuildAndLoadAsync();
	_buildFuture = then(_buildFuture, [this](bool result) {
		if (result) {
			StartPlaying();
		}
		// TODO: Handle build failure in the UI.
		return result;
	});
}

void MapController::StartPlaying()
{
	// Make sure we don't update the map while we're changing the map.
	_updateMapLock.lock();

	// Ensure sound will work
	auto audioSys = AudioSystem::GetInstance();
	audioSys->Resume(-1);
	audioSys->ResumeMusic();
	
	_playState = PLAYING;

	// Copy the original map (so we don't break the original during play)
	_originalMap = _editor->GetSelectedMap();
	_previewMap = Factory::Copy<Map>(_originalMap->GetName(), _originalMap);

	auto mapManager = MapManager::GetInstance();

	// Set the copy as the "selected" map (so both the editor and scripting engine know)
	mapManager->SetCurrentMap(_previewMap);
	_editor->SetSelectedMap(_previewMap);

  // Remove selection as it might reference the original map.
  // TODO: Select the copy of the selected entity instead.
  _editor->SetSelectedEntity(nullptr);

	_updateMapLock.unlock();
}

void MapController::Pause()
{
	if (_editor->GetSelectedMap() == nullptr)
		return;

	if (_playState == PAUSED)
	{
		Continue();
		return;
	}

	_playState = PAUSED;

	// Pause all sound
	auto audioSys = AudioSystem::GetInstance();
	audioSys->Pause(-1);
	audioSys->PauseMusic();
}

void MapController::Continue()
{
	if (_playState != PAUSED || _editor->GetSelectedMap() == nullptr)
		return;

	_playState = PLAYING;

	// Resume all sound
	auto audioSys = AudioSystem::GetInstance();
	audioSys->Resume(-1);
	audioSys->ResumeMusic();
}

void MapController::Stop()
{
	if (_playState == STOPPED || _editor->GetSelectedMap() == nullptr)
		return;

	// Make sure we don't update the map while we're changing the map.
	_updateMapLock.lock();

	_playState = STOPPED;

	// Set selected entity to null so the inspector doesn't try to inspect it
	_editor->SetSelectedEntity(nullptr);

	// Delete the copy map
	delete _previewMap;
	_previewMap = nullptr;

	// Put the original map back as "selected"
	auto mapManager = MapManager::GetInstance();
	mapManager->SetCurrentMap(_originalMap);
	_editor->SetSelectedMap(_originalMap);

	// Stop all audio
	auto audioSys = AudioSystem::GetInstance();
	audioSys->Stop();
	audioSys->StopMusic();

	_updateMapLock.unlock();
}

PlayState MapController::GetPlayState() const
{
  return _playState;
}

MapController::~MapController() 
{
}