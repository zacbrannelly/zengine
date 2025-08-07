#pragma once

#include <bgfx/bgfx.h>

namespace ZEngine
{
  class Graphics;

  class FrameBuffer
  {
  public:
    FrameBuffer();
    virtual ~FrameBuffer();

    // Build normal frame buffer with color and depth attachments.
    void Build(int width, int height);

    // Build frame buffer with single texture attachment (useful for shadow maps).
    void Build(bgfx::TextureHandle texture, int layerIdx = 0);

    // Allow resizing of the frame buffer.
    void Resize(int width, int height);

    bgfx::FrameBufferHandle GetHandle() const;
    bgfx::TextureHandle GetTexture() const;
    int GetTextureLayer() const;
    bool IsValid() const;
  private:
    bgfx::FrameBufferHandle _handle;
    bgfx::TextureHandle _texture;
    int _layerIdx { 0 };
    bool _resizable { false };
    Graphics* _graphics { nullptr };
  };
}
