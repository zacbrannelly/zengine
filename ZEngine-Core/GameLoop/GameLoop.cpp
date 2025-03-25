#include "GameLoop.h"
#include "../Physics/Time.h"
#include "../Display/Display.h"
#include "../Input/InputManager.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

using namespace ZEngine;

GameLoop::GameLoop(
  Display* display,
  double dt,
  std::function<void()> updateCallback,
  std::function<void()> renderCallback
) :
  _dt(dt),
  _display(display),
  _updateCallback(updateCallback),
  _renderCallback(renderCallback),
  _currentTime(0.0),
  _accumulator(0.0),
  _newTime(0.0),
  _frameTime(0.0)
{
  _inputManager = InputManager::GetInstance();
  _time = Time::GetInstance();
}

GameLoop::~GameLoop()
{
}

void GameLoop::FullStep() 
{
  _newTime = _time->GetTime();
  _frameTime = _newTime - _currentTime;
  _currentTime = _newTime;

  _accumulator += _frameTime;

  while (_accumulator >= _dt)
  {
    UpdateGame();
    _accumulator -= _dt;
  }

  if (_renderCallback)
  {
    _renderCallback();
  }
}

void GameLoop::StartLoop()
{
  _currentTime = _time->GetTime();

#ifdef __EMSCRIPTEN__
  emscripten_cancel_main_loop();
  emscripten_set_main_loop_arg([](void* gameLoop) {
    static_cast<GameLoop*>(gameLoop)->FullStep();
  }, this, 0, 1);
#else
  while (!CloseRequested())
  {
    FullStep();
  }
#endif
}

void GameLoop::UpdateGame()
{
  _inputManager->Reset();
  _display->Update();

  if (_updateCallback)
  {
    _updateCallback();
  }

  _time->Tick();
}

bool GameLoop::CloseRequested() const
{
  return _display->CloseRequested();
}
