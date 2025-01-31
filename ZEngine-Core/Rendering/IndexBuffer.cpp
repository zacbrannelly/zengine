#include "IndexBuffer.h"

using namespace bgfx;
using namespace ZEngine;

IndexBuffer::IndexBuffer()
{
}

void IndexBuffer::Upload(const unsigned int* indices, unsigned int size, bool copy)
{
	Memory* memory = nullptr;

	if (copy)
		memory = (Memory*)bgfx::copy(indices, size);
	else
		memory = (Memory*)bgfx::makeRef(indices, size);

	auto handle = bgfx::createIndexBuffer(memory, BGFX_BUFFER_INDEX32);

	SetHandleID(handle.idx);
}

IndexBuffer::~IndexBuffer()
{
	destroy(GetHandle());
}
