#include "GameLoop.h"
#include "../Physics/Time.h"
#include "../Display/Display.h"
#include "../Input/InputManager.h"

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

void GameLoop::StartLoop()
{
  _currentTime = _time->GetTime();

  while (!CloseRequested())
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
