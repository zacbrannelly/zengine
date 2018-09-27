#include "DynamicIndexBuffer.h"

using namespace bgfx;

DynamicIndexBuffer::DynamicIndexBuffer()
{
}

void DynamicIndexBuffer::Upload(const unsigned int* indices, unsigned int size, bool copy)
{
	Memory* memory = nullptr;

	if (copy)
		memory = (Memory*)bgfx::copy(indices, size);
	else
		memory = (Memory*)bgfx::makeRef(indices, size);

	if (GetHandle().idx == kInvalidHandle)
	{
		auto handle = createDynamicIndexBuffer(memory, BGFX_BUFFER_INDEX32);
		SetHandleID(handle.idx);
	}
	else
		Update(0, indices, size, copy);
}

void DynamicIndexBuffer::Update(unsigned int startIndex, const unsigned int* data, unsigned int size, bool copy)
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

