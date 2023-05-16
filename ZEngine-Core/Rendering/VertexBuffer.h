#pragma once

#include <bgfx/bgfx.h>
#include "BufferHandle.h"

class VertexBuffer : public BufferHandle<bgfx::VertexBufferHandle>
{
public:
	VertexBuffer(bgfx::VertexLayout decl);
	virtual ~VertexBuffer();

	virtual void Upload(const void* data, unsigned int size, bool copy);

	bgfx::VertexLayout GetDecl() const;
private:
	bgfx::VertexLayout _decl;
};

