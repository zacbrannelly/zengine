#pragma once

#include <bgfx/bgfx.h>
#include "BufferHandle.h"

class IndexBuffer : public BufferHandle<bgfx::IndexBufferHandle>
{
public:
	IndexBuffer();
	~IndexBuffer();

	virtual void Upload(const unsigned short* indices, unsigned int size, bool copy);
};

