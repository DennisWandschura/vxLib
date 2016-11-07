#pragma once

#include <vxLib/Stream.h>
#include <vxLib/Allocator/Allocator.h>
#include <vxLib/math/math.h>

namespace vx
{
	template<typename Allocator>
	class BufferOutStream : public OutStream
	{
		u8* m_head;
		u8* m_last;
		AllocatedBlock m_data;
		Allocator m_allocator;

		void grow(size_t capacity)
		{
			VX_ASSERT(capacity != 0);

			auto newData = m_allocator.allocate(capacity, 16llu);

			::memcpy(newData.ptr, m_data.ptr, m_data.size);

			auto headOffset = m_head - m_data.ptr;

			m_allocator.deallocate(m_data);
			m_data = newData;

			m_head = m_data.ptr + headOffset;
			m_last = m_data.ptr + m_data.size;
		}

	public:
		BufferOutStream() :OutStream(), m_head(nullptr), m_last(nullptr), m_data({ nullptr, 0 }), m_allocator() {}

		BufferOutStream(Allocator &&alloc, size_t capacity) :OutStream(), m_head(nullptr), m_last(nullptr), m_data(), m_allocator(std::move(alloc))
		{
			capacity = nextPowerOf2(capacity);
			m_data = m_allocator.allocate(capacity, 16llu);
			VX_ASSERT(m_data.ptr != nullptr);
			m_head = m_data.ptr;
			m_last = m_data.ptr + m_data.size;
		}

		~BufferOutStream()
		{
			m_allocator.deallocate(m_data);
		}

		void initialize(Allocator &&alloc, size_t capacity)
		{
			VX_ASSERT(m_data.ptr == nullptr);
			m_allocator = std::move(alloc);

			capacity = nextPowerOf2(capacity);
			m_data = m_allocator.allocate(capacity, 16llu);
			VX_ASSERT(m_data.ptr != nullptr);
			m_head = m_data.ptr;
			m_last = m_data.ptr + m_data.size;
		}

		s32 write(const u8* src, s32 size) override
		{
			auto newHead = m_head + size;
			if (newHead >= m_last)
			{
				grow(m_data.size * 2);
			}

			::memcpy(m_head, src, size);
			m_head += size;

			return size;
		}

		size_t size() const { return (m_head - m_data.ptr); }
		const u8* data() const { return m_data.ptr; }
	};

	template<typename Allocator>
	class BufferInStream : public InStream
	{
		u8* m_head;
		u8* m_last;
		AllocatedBlock m_data;
		Allocator m_allocator;

	public:
		BufferInStream() :InStream(), m_head(nullptr), m_last(nullptr), m_data({ nullptr, 0 }), m_allocator() {}

		BufferInStream(Allocator &&alloc, const u8* src, size_t size) :InStream(), m_head(nullptr), m_last(nullptr), m_data(), m_allocator(std::move(alloc))
		{
			m_data = m_allocator.allocate(size, 16llu);
			VX_ASSERT(m_data.ptr != nullptr);
			m_head = m_data.ptr;
			m_last = m_data.ptr + size;

			::memcpy(m_data.ptr, src, size);
		}

		~BufferInStream()
		{
			m_allocator.deallocate(m_data);
		}

		void initialize(Allocator &&alloc, const u8* src, size_t size)
		{
			VX_ASSERT(m_data.ptr == nullptr);
			m_allocator = std::move(alloc);

			m_data = m_allocator.allocate(size, 16llu);
			VX_ASSERT(m_data.ptr != nullptr);
			m_head = m_data.ptr;
			m_last = m_data.ptr + size;

			::memcpy(m_data.ptr, src, size);
		}

		s32 read(u8* dst, s32 readSize) override
		{
			auto newHead = m_head + readSize;

			VX_ASSERT(newHead <= m_last);

			::memcpy(dst, m_head, readSize);
			m_head = newHead;

			return readSize;
		}

		size_t size() const { return (m_head - m_data.ptr); }
		const u8* data() const { return m_data.ptr; }
	};
}