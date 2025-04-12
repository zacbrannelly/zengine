#include "StandardShaders.h"
#include "Shader.h"
#include "StandardShaders/UnlitColorShader.h"
#include "StandardShaders/UnlitTextureShader.h"
#include "../Logging/LoggingSystem.h"

#include <map>

using namespace ZEngine;

Shader* StandardShaders::_unlitColorShader = nullptr;
Shader* StandardShaders::_unlitTextureShader = nullptr;

static const std::string MODUlE_NAME = "StandardShaders";

static std::map<std::string, StandardShader> shaderNameToEnum = {
  { "UNLIT_COLOR_SHADER",   UNLIT_COLOR_SHADER   },
  { "UNLIT_TEXTURE_SHADER", UNLIT_TEXTURE_SHADER },
};

void StandardShaders::Init()
{
  LoadUnlitColorShader();
  LoadUnlitTextureShader();
}

Shader* StandardShaders::GetShader(const std::string& shaderName)
{
  auto it = shaderNameToEnum.find(shaderName);
  if (it != shaderNameToEnum.end())
  {
    return GetShader(it->second);
  }
  else
  {
    LoggingSystem::GetInstance()->LogError("GetShader: Shader not found: " + shaderName, MODUlE_NAME);
    return nullptr;
  }
}

Shader* StandardShaders::GetShader(StandardShader shader)
{
  switch (shader)
  {
    case UNLIT_COLOR_SHADER:
      return _unlitColorShader;
    case UNLIT_TEXTURE_SHADER:
      return _unlitTextureShader;
    default:
      return nullptr;
  }
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
    LoggingSystem::GetInstance()->LogInfo("Loaded Unlit Color Shader.", MODUlE_NAME);
  }
  else
  {
    LoggingSystem::GetInstance()->LogError("LoadUnlitColorShader: Failed to load.", MODUlE_NAME);
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
    LoggingSystem::GetInstance()->LogInfo("Loaded Unlit Texture Shader.", MODUlE_NAME);
  }
  else
  {
    LoggingSystem::GetInstance()->LogError("LoadUnlitTextureShader: Failed to load.", MODUlE_NAME);
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
