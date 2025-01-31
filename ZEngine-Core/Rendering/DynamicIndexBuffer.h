#pragma once

#include "IndexBuffer.h"

namespace ZEngine
{
	class DynamicIndexBuffer : public IndexBuffer
	{
	public:
		DynamicIndexBuffer();
		~DynamicIndexBuffer();

		void Upload(const unsigned int* indices, unsigned int size, bool copy) override;
		void Update(unsigned int startIndex, const unsigned int* data, unsigned int size, bool copy);
	};
}
