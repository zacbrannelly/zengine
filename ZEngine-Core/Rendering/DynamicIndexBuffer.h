#pragma once

#include "IndexBuffer.h"

class DynamicIndexBuffer : public IndexBuffer
{
public:
	DynamicIndexBuffer();
	~DynamicIndexBuffer();

	void Upload(const unsigned short* indices, unsigned int size, bool copy) override;
	void Update(unsigned int startIndex, const unsigned short* data, unsigned int size, bool copy);
};

