#pragma once

#include <string>
#include <vector>

namespace ZEngine
{
  enum StandardShader
  {
    UNLIT_COLOR_SHADER,
    UNLIT_TEXTURE_SHADER,
  };

  class Shader;

  struct ShaderInfo
  {
    std::string enumName;
    StandardShader enumValue;
    Shader* shader;
  };

  class StandardShaders
  {
  public:
    static void Init();
    static void Release();

    static Shader* GetShader(const std::string& enumName);
    static Shader* GetShader(StandardShader shader);

    static Shader* GetUnlitColorShader();
    static Shader* GetUnlitTextureShader();

    static const std::vector<ShaderInfo>& GetShaders();

  private:
    static Shader* _unlitColorShader;
    static Shader* _unlitTextureShader;
    static std::vector<ShaderInfo> _shaders;

    static void LoadUnlitColorShader();
    static void LoadUnlitTextureShader();
  };
}
