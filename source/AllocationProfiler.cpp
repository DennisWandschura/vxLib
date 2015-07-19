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

#include <vxLib/Allocator/AllocationProfiler.h>

namespace vx
{
	struct AllocationProfiler::Entry
	{
		u32 totalSize;
		u32 usedSize;
		u32 maxSize;
		u32 minAllocSize;
		u32 maxAllocSize;
		char name[32];
	};

	AllocationProfiler::AllocationProfiler()
		:m_entries()
	{
	}

	AllocationProfiler::~AllocationProfiler()
	{

	}

	void AllocationProfiler::registerAllocator(const vx::Allocator* allocator, const char* text)
	{
		Entry entry{};
		entry.maxSize = 0;
		entry.totalSize = allocator->getTotalSize();
		entry.minAllocSize = 0xffffffff;
		entry.maxAllocSize = 0;

		auto size = strnlen(text, 31);
		strncpy(entry.name, text, size);

		auto ptr = allocator->getMemory();

		m_entries.insert(ptr, entry);
	}

	void AllocationProfiler::updateAllocation(const u8* ptr, u32 size)
	{
		auto it = m_entries.find(ptr);
		if (it != m_entries.end())
		{
			it->usedSize += size;
			it->maxSize = std::max(it->maxSize, it->usedSize);
			it->maxAllocSize = std::max(it->maxAllocSize, size);
			it->minAllocSize = std::min(it->minAllocSize, size);
		}
	}

	void AllocationProfiler::updateDeallocation(const u8* ptr, u32 size)
	{
		auto it = m_entries.find(ptr);
		if (it != m_entries.end())
		{
			it->usedSize -= size;
		}
	}

	void AllocationProfiler::print() const
	{
		printf("\nRegistered Allocators:\n-------------------\n");
		for (auto &it : m_entries)
		{
			printf("Name: %s, currentSize: %u, totalSize: %u, maxSize: %u, minAlloc: %u, maxAlloc: %u\n", it.name, it.usedSize, it.totalSize, it.maxSize, it.minAllocSize, it.maxAllocSize);
		}
	}
}