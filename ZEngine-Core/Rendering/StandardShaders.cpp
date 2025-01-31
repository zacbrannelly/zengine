#include "StandardShaders.h"
#include "Shader.h"
#include "StandardShaders/UnlitColorShader.h"

#include <iostream>

using namespace ZEngine;

Shader* StandardShaders::_unlitColorShader = nullptr;

void StandardShaders::Init()
{
  LoadUnlitColorShader();
}

void StandardShaders::LoadUnlitColorShader()
{
  _unlitColorShader = new Shader("Unlit Color");
  if (_unlitColorShader->Load(
    (uint8_t*)UNLIT_COLOR_VERTEX_SHADER,
    (uint32_t)sizeof(UNLIT_COLOR_VERTEX_SHADER),
    (uint8_t*)UNLIT_COLOR_FRAGMENT_SHADER,
    (uint32_t)sizeof(UNLIT_COLOR_FRAGMENT_SHADER)
  ))
  {
    std::cout << "STANDARD SHADERS: Loaded Unlit Color Shader" << std::endl;
  }
  else
  {
    std::cout << "STANDARD SHADERS: Failed to load Unlit Color Shader" << std::endl;
  }
}

void StandardShaders::Release()
{
  if (_unlitColorShader != nullptr)
  {
    _unlitColorShader->Release();
    delete _unlitColorShader;
    _unlitColorShader = nullptr;
  }
}

Shader* StandardShaders::GetUnlitColorShader()
{
  return _unlitColorShader;
}
