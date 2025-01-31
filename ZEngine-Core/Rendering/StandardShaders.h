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

  private:
    static Shader* _unlitColorShader;

    static void LoadUnlitColorShader();
  };
}
