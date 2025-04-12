#include "StandardShaders.h"
#include "Shader.h"
#include "StandardShaders/UnlitColorShader.h"
#include "StandardShaders/UnlitTextureShader.h"

#include <iostream>

using namespace ZEngine;

Shader* StandardShaders::_unlitColorShader = nullptr;
Shader* StandardShaders::_unlitTextureShader = nullptr;

void StandardShaders::Init()
{
  LoadUnlitColorShader();
  LoadUnlitTextureShader();
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

void StandardShaders::LoadUnlitTextureShader()
{
  _unlitTextureShader = new Shader("Unlit Texture");
  if (_unlitTextureShader->Load(
    (uint8_t*)UNLIT_TEXTURE_VERTEX_SHADER,
    (uint32_t)sizeof(UNLIT_TEXTURE_VERTEX_SHADER),
    (uint8_t*)UNLIT_TEXTURE_FRAGMENT_SHADER,
    (uint32_t)sizeof(UNLIT_TEXTURE_FRAGMENT_SHADER)
  ))
  {
    std::cout << "STANDARD SHADERS: Loaded Unlit Texture Shader" << std::endl;
  }
  else
  {
    std::cout << "STANDARD SHADERS: Failed to load Unlit Texture Shader" << std::endl;
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

  if (_unlitTextureShader != nullptr)
  {
    _unlitTextureShader->Release();
    delete _unlitTextureShader;
    _unlitTextureShader = nullptr;
  }
}

Shader* StandardShaders::GetUnlitColorShader()
{
  return _unlitColorShader;
}

Shader* StandardShaders::GetUnlitTextureShader()
{
  return _unlitTextureShader;
}
