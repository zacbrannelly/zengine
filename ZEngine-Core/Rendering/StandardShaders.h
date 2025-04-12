#pragma once

#include <string>

namespace ZEngine
{
  enum StandardShader
  {
    UNLIT_COLOR_SHADER,
    UNLIT_TEXTURE_SHADER,
  };

  class Shader;

  class StandardShaders
  {
  public:
    static void Init();
    static void Release();

    static Shader* GetShader(const std::string& enumName);
    static Shader* GetShader(StandardShader shader);

    static Shader* GetUnlitColorShader();
    static Shader* GetUnlitTextureShader();

  private:
    static Shader* _unlitColorShader;
    static Shader* _unlitTextureShader;

    static void LoadUnlitColorShader();
    static void LoadUnlitTextureShader();
  };
}
