
#pragma once

#include <mutex>
#include <future>
#include "Controller.h"

class Map;
class Editor;
class Physics3DSystem;

enum PlayState {
  PLAYING,
  BUILDING,
  PAUSED,
  STOPPED
};

class MapController : public Controller
{
public:
  MapController(Editor* editor);
  virtual ~MapController();

  void Update() override;

  void Play();
	void Pause();
	void Continue();
	void Stop();

  PlayState GetPlayState() const;

private:
	void StartPlaying();

  Editor* _editor;
  Map* _previewMap;
	Map* _originalMap;
  PlayState _playState;
  std::future<bool> _buildFuture;
	std::mutex _updateMapLock;
  bool _playWhenPossible;
  Physics3DSystem* _physics;
};
