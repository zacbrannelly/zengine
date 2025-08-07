#include "FrameBuffer.h"
#include "Graphics.h"

using namespace ZEngine;

FrameBuffer::FrameBuffer()
{
  _handle = bgfx::FrameBufferHandle { bgfx::kInvalidHandle };
  _graphics = Graphics::GetInstance();
}

void FrameBuffer::Build(int width, int height)
{
  _handle = _graphics->CreateFrameBuffer(width, height);
  _texture = _graphics->GetFrameBufferTexture(_handle);
  _resizable = true;
}

void FrameBuffer::Build(bgfx::TextureHandle texture, int layerIdx)
{
  bgfx::Attachment attachment;
  attachment.init(texture, bgfx::Access::Write, layerIdx);
  _handle = _graphics->CreateFrameBuffer(attachment);
  _texture = texture;
  _layerIdx = layerIdx;
  _resizable = false;
}

void FrameBuffer::Resize(int width, int height)
{
  if (_resizable)
  {
    _graphics->DeleteFrameBuffer(_handle);
    Build(width, height);
  }
}

bgfx::FrameBufferHandle FrameBuffer::GetHandle() const
{
  return _handle;
}

bgfx::TextureHandle FrameBuffer::GetTexture() const
{
  return _texture;
}

int FrameBuffer::GetTextureLayer() const
{
  return _layerIdx;
}

bool FrameBuffer::IsValid() const
{
  return _handle.idx != bgfx::kInvalidHandle;
}

FrameBuffer::~FrameBuffer()
{
  if (_handle.idx != bgfx::kInvalidHandle)
  {
    bgfx::destroy(_handle);
  }
}
