#pragma once

#include "VertexBuffer.h"

class DynamicVertexBuffer : public VertexBuffer
{
public:
	DynamicVertexBuffer(bgfx::VertexLayout decl);
	~DynamicVertexBuffer();

	void Upload(const void* data, unsigned int size, bool copy) override;
	void Update(unsigned int startIndex, const void* data, unsigned int size, bool copy);
};

