#pragma once

#include <bgfx/bgfx.h>
#include "BufferHandle.h"

namespace ZEngine
{
	class IndexBuffer : public BufferHandle<bgfx::IndexBufferHandle>
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		virtual void Upload(const unsigned int* indices, unsigned int size, bool copy);
	};
}
