#pragma once

namespace ZEngine
{
  class Shader;

  class StandardShaders
  {
  public:
    static void Init();
    static void Release();

    static Shader* GetUnlitColorShader();
    static Shader* GetUnlitTextureShader();

  private:
    static Shader* _unlitColorShader;
    static Shader* _unlitTextureShader;

    static void LoadUnlitColorShader();
    static void LoadUnlitTextureShader();
  };
}
