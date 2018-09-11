#include "VertexBuffer.h"

using namespace bgfx;

VertexBuffer::VertexBuffer(VertexDecl decl)
{
	_decl = decl;
}

void VertexBuffer::Upload(const void* data, unsigned int size, bool copy)
{
	Memory* memory = nullptr;

	if (copy)
		memory = (Memory*)bgfx::copy(data, size);
	else
		memory = (Memory*)bgfx::makeRef(data, size);

	auto handle = createVertexBuffer(memory, _decl);
	SetHandleID(handle.idx);
}

VertexDecl VertexBuffer::GetDecl() const
{
	return _decl;
}

VertexBuffer::~VertexBuffer()
{
	destroy(GetHandle());
}
