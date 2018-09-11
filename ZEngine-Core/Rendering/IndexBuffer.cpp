#include "IndexBuffer.h"

using namespace bgfx;

IndexBuffer::IndexBuffer()
{
}

void IndexBuffer::Upload(const unsigned short* indices, unsigned int size, bool copy)
{
	Memory* memory = nullptr;

	if (copy)
		memory = (Memory*)bgfx::copy(indices, size);
	else
		memory = (Memory*)bgfx::makeRef(indices, size);

	auto handle = bgfx::createIndexBuffer(memory);
	SetHandleID(handle.idx);
}

IndexBuffer::~IndexBuffer()
{
	destroy(GetHandle());
}
