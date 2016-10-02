#pragma once

/*
The MIT License (MIT)

Copyright (c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <vxLib/Allocator/GpuAllocator.h>

namespace vx
{
	class GpuLinearAllocator
	{
		u64 m_head;
		u64 m_capacity;

	public:
		typedef u64 Marker;

		GpuLinearAllocator() :m_head(0), m_capacity(0){}
		~GpuLinearAllocator() {}

		void initialize(u64 capacity)
		{
			m_capacity = capacity; 
		}

		vx::GpuAllocatedBlock allocate(u64 size, u64 alignment)
		{
			auto alignedSize = vx::getAlignedSize(size, alignment);
			auto alignedHead = vx::getAlignedSize(m_head, alignment);

			auto newHead = alignedHead + alignedSize;
			if (newHead > m_capacity)
			{
				return{ 0, 0 };
			}

			m_head = newHead;

			return{ alignedHead, alignedSize };
		}

		void deallocate(const vx::GpuAllocatedBlock &block)
		{
			if (block.size == 0)
				return;

			auto tmp = block.offset + block.size;
			if (tmp == m_head)
			{
				m_head = block.offset;
			}
		}

		Marker getMarker() { return m_head; }

		void setHeadToCapacity()
		{
			m_head = m_capacity;
		}

		void clearToMarker(Marker marker)
		{
			if(m_head > marker)
				m_head = marker;
		}

		void clearAll()
		{
			m_head = 0;
		}

		u64 capacity() const { return m_capacity; }
	};
}