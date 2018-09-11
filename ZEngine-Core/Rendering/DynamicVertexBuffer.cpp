#include "DynamicVertexBuffer.h"

using namespace bgfx;

DynamicVertexBuffer::DynamicVertexBuffer(bgfx::VertexDecl decl) : VertexBuffer(decl)
{
}

void DynamicVertexBuffer::Upload(const void * data, unsigned int size, bool copy)
{
	Memory* memory = nullptr;

	if (copy)
		memory = (Memory*)bgfx::copy(data, size);
	else
		memory = (Memory*)bgfx::makeRef(data, size);

	if (GetHandle().idx == kInvalidHandle)
	{
		auto handle = createDynamicVertexBuffer(memory, GetDecl());
		SetHandleID(handle.idx);
	}
	else
		Update(0, data, size, copy);
}
 
void DynamicVertexBuffer::Update(unsigned int startIndex, const void* data, unsigned int size, bool copy)
{
	Memory* memory = nullptr;

	if (copy)
		memory = (Memory*)bgfx::copy(data, size);
	else
		memory = (Memory*)bgfx::makeRef(data, size);

	DynamicVertexBufferHandle handle = { GetHandle().idx };
	update(handle, startIndex, memory);
}

DynamicVertexBuffer::~DynamicVertexBuffer()
{
}

