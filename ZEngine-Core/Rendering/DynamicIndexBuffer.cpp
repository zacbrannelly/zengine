#include "DynamicIndexBuffer.h"

using namespace bgfx;

DynamicIndexBuffer::DynamicIndexBuffer()
{
}

void DynamicIndexBuffer::Upload(const unsigned short * indices, unsigned int size, bool copy)
{
	Memory* memory = nullptr;

	if (copy)
		memory = (Memory*)bgfx::copy(indices, size);
	else
		memory = (Memory*)bgfx::makeRef(indices, size);

	if (GetHandle().idx == kInvalidHandle)
	{
		auto handle = createDynamicIndexBuffer(memory);
		SetHandleID(handle.idx);
	}
	else
		Update(0, indices, size, copy);
}

void DynamicIndexBuffer::Update(unsigned int startIndex, const unsigned short* data, unsigned int size, bool copy)
{
	Memory* memory = nullptr;

	if (copy)
		memory = (Memory*)bgfx::copy(data, size);
	else
		memory = (Memory*)bgfx::makeRef(data, size);

	DynamicIndexBufferHandle handle = { GetHandle().idx };
	update(handle, 0, memory);
}

DynamicIndexBuffer::~DynamicIndexBuffer()
{
}

