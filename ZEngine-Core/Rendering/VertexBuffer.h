#pragma once

#include <bgfx/bgfx.h>
#include "BufferHandle.h"

class VertexBuffer : public BufferHandle<bgfx::VertexBufferHandle>
{
public:
	VertexBuffer(bgfx::VertexDecl decl);
	virtual ~VertexBuffer();

	virtual void Upload(const void* data, unsigned int size, bool copy);

	bgfx::VertexDecl GetDecl() const;
private:
	bgfx::VertexDecl _decl;
};

