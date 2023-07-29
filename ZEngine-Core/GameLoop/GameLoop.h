#pragma once

#include <functional>

class Display;
class InputManager;
class Time;

class GameLoop
{
public:
  GameLoop(
    Display* display,
    double dt = 1.0 / 60.0,
    std::function<void()> updateCallback = nullptr,
    std::function<void()> renderCallback = nullptr
  );
  ~GameLoop();

  void StartLoop();
  void UpdateGame();
  
  bool CloseRequested() const;

private:
  double _dt;
  double _currentTime;
  double _accumulator;
  double _newTime;
  double _frameTime;
  std::function<void()> _updateCallback;
  std::function<void()> _renderCallback;

  Display* _display;
  InputManager* _inputManager;
  Time* _time;
};
