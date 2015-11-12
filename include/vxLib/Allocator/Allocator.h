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

#include <vxLib/types.h>

template<size_t SIZE, size_t ALIGNMENT>
struct GetAlignedSize
{
	enum { size = (SIZE + (ALIGNMENT - 1) & ~(ALIGNMENT - 1)) };
};

inline size_t getAlignedSize(size_t size, size_t alignment)
{
	return (size + (alignment - 1) & ~(alignment - 1));
}

inline u8* getAlignedPtr(u8* ptr, size_t alignment)
{
	return (u8*)getAlignedSize((size_t)ptr, alignment);
}

struct AllocatedBlock
{
	u8* ptr;
	size_t size;
};

template<typename T>
class Memory
{
protected:
	inline Memory() {}

	inline ~Memory() {}

	T& get()
	{
		return static_cast<T&>(*this);
	}

	const T& get() const
	{
		return static_cast<const T&>(*this);
	}

public:
	u8* aquire(size_t* size)
	{
		return get().aquire(size);
	}
};

class HeapMemory : public Memory<HeapMemory>
{
	u8* m_data;
	size_t m_size;
	size_t m_aquired;

public:
	HeapMemory() :m_data(nullptr), m_size(0), m_aquired(0){}

	HeapMemory(const HeapMemory&) = delete;

	~HeapMemory()
	{
		if (m_data)
		{
			_aligned_free(m_data);
			m_data = nullptr;
			m_size = 0;
		}
	}

	HeapMemory& operator=(const HeapMemory&) = delete;

	void initialize(size_t size, size_t alignment)
	{
		auto alignedSize = getAlignedSize(size, alignment);
		m_data = (u8*)_aligned_malloc(alignedSize, alignment);
		m_size = alignedSize;
	}

	u8* aquire(size_t* size)
	{
		if (m_aquired == 0)
		{
			m_aquired = 1;

			*size = m_size;
			return m_data;
		}
		
		return nullptr;
	}
};

template<size_t SIZE, size_t ALIGNMENT>
class alignas(ALIGNMENT) StackMemory : public Memory<StackMemory<SIZE, ALIGNMENT>>
{
	enum : size_t { alignedSize = GetAlignedSize<SIZE, ALIGNMENT>::size };

	u8 m_data[alignedSize];
	size_t m_aquired;

public:
	inline StackMemory():m_data(), m_aquired(0){}
	inline ~StackMemory() {}

	u8* aquire(size_t* size)
	{
		if (m_aquired == 0)
		{
			*size = alignedSize;
			return m_data;
		}

		return nullptr;
	}
};

template<typename T>
struct Allocator
{
private:
	T& get() { return static_cast<T&>(*this); }

public:
	AllocatedBlock allocate(size_t size, size_t alignment)
	{
		return get().allocate(size, alignment);
	}

	void deallocate(const AllocatedBlock &block)
	{
		return get().deallocate(block);
	}

	void deallocateAll()
	{
		return get().deallocateAll();
	}

	bool contains(const AllocatedBlock &block) const
	{
		return get().contains(block);
	}
};