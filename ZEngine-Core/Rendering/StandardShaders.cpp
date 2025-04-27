#include "StandardShaders.h"
#include "Shader.h"
#include "StandardShaders/UnlitColorShader.h"
#include "StandardShaders/UnlitTextureShader.h"
#include "StandardShaders/LitColorShader.h"
#include "../Logging/LoggingSystem.h"

#include <map>

using namespace ZEngine;

Shader* StandardShaders::_unlitColorShader = nullptr;
Shader* StandardShaders::_unlitTextureShader = nullptr;
Shader* StandardShaders::_litColorShader = nullptr;
std::vector<ShaderInfo> StandardShaders::_shaders;

static const std::string MODUlE_NAME = "StandardShaders";

static std::map<std::string, StandardShader> shaderNameToEnum;

void StandardShaders::Init()
{
  LoadUnlitColorShader();
  LoadUnlitTextureShader();
  LoadLitColorShader();

  // Build the shader name to enum map
  for (const auto& info : _shaders)
  {
    shaderNameToEnum[info.enumName] = info.enumValue;
  }
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
    case LIT_COLOR_SHADER:
      return _litColorShader;
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
    _shaders.push_back({ "UNLIT_COLOR_SHADER", UNLIT_COLOR_SHADER, _unlitColorShader });
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
    _shaders.push_back({ "UNLIT_TEXTURE_SHADER", UNLIT_TEXTURE_SHADER, _unlitTextureShader });
    LoggingSystem::GetInstance()->LogInfo("Loaded Unlit Texture Shader.", MODUlE_NAME);
  }
  else
  {
    LoggingSystem::GetInstance()->LogError("LoadUnlitTextureShader: Failed to load.", MODUlE_NAME);
  }
}

void StandardShaders::LoadLitColorShader()
{
  _litColorShader = new Shader("Lit Color");
  if (_litColorShader->Load(
    (uint8_t*)LIT_COLOR_VERTEX_SHADER,
    (uint32_t)sizeof(LIT_COLOR_VERTEX_SHADER),
    (uint8_t*)LIT_COLOR_FRAGMENT_SHADER,
    (uint32_t)sizeof(LIT_COLOR_FRAGMENT_SHADER)
  ))
  {
    _shaders.push_back({ "LIT_COLOR_SHADER", LIT_COLOR_SHADER, _litColorShader });
    LoggingSystem::GetInstance()->LogInfo("Loaded Lit Color Shader.", MODUlE_NAME);
  }
  else
  {
    LoggingSystem::GetInstance()->LogError("LoadLitColorShader: Failed to load.", MODUlE_NAME);
  }
}

void StandardShaders::Release()
{
  for (auto info : _shaders)
  {
    if (info.shader != nullptr)
    {
      info.shader->Release();
      delete info.shader;
    }
  }

  _shaders.clear();
  _unlitColorShader = nullptr;
  _unlitTextureShader = nullptr;
  _litColorShader = nullptr;
}

Shader* StandardShaders::GetUnlitColorShader()
{
  return _unlitColorShader;
}

Shader* StandardShaders::GetUnlitTextureShader()
{
  return _unlitTextureShader;
}

const std::vector<ShaderInfo>& StandardShaders::GetShaders()
{
  return _shaders;
}
