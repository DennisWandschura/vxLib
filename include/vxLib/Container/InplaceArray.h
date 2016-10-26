#pragma once

/*
The MIT License (MIT)

Copyright (c) 2016 Dennis Wandschura

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

#include <vxLib/Container/ArrayBase.h>
#include <vxLib/ArrayAnalyzer.h>
#include <vxLib/TypeInfo.h>
#include <vxLib/string.h>

namespace vx
{
	template<typename T, size_t COUNT, typename Allocator = Mallocator>
	class InplaceArray : public ArrayBase<T>
	{
		typedef Allocator MyAllocator;
		typedef ArrayBase<T> MyBase;

		enum {BUFFER_SIZE = sizeof(T) * COUNT };

		u8 m_data[BUFFER_SIZE];
		size_t m_blockSize;
		MyAllocator m_allocator;
#ifdef _VX_ARRAY_ANALYZER
		ArrayStats m_arrayStats;

		ArrayStats& getArrayStats() { return m_arrayStats; }
#endif

	public:
		typedef typename MyBase::value_type value_type;
		typedef typename MyBase::pointer pointer;
		typedef typename MyBase::const_pointer const_pointer;
		typedef typename MyBase::reference reference;
		typedef typename MyBase::const_reference const_reference;

		typedef typename MyBase::iterator iterator;
		typedef typename MyBase::const_iterator const_iterator;

		InplaceArray()
			:MyBase(reinterpret_cast<pointer>(m_data), reinterpret_cast<pointer>(m_data + BUFFER_SIZE)), 
			m_data(), 
			m_blockSize(0),
			m_allocator() 
		{
			VX_REGISTER_ANALYZER;
		}

		InplaceArray(MyAllocator &&allocator) 
			:MyBase(reinterpret_cast<pointer>(m_data), reinterpret_cast<pointer>(m_data + BUFFER_SIZE)),
			m_data(),
			m_blockSize(0), 
			m_allocator(std::move(allocator))
		{
			VX_REGISTER_ANALYZER;
		}

		InplaceArray(MyAllocator &&allocator, u32 capacity) 
			:MyBase(reinterpret_cast<pointer>(m_data), reinterpret_cast<pointer>(m_data + BUFFER_SIZE)),
			m_data(), 
			m_blockSize(0), 
			m_allocator(std::move(allocator))
		{
			VX_REGISTER_ANALYZER;
			reserve(capacity);
		}

		template<size_t OtherCount, typename OtherAlloc>
		InplaceArray(MyAllocator &&allocator, const InplaceArray<T, OtherCount, OtherAlloc> &rhs)
			:InplaceArray(std::move(allocator), rhs.capacity())
		{
			VX_REGISTER_ANALYZER;

			push_back_range(rhs.begin(), rhs.end());
		}

		InplaceArray(InplaceArray &&rhs)
			: MyBase(reinterpret_cast<pointer>(m_data), reinterpret_cast<pointer>(m_data + BUFFER_SIZE)),
			m_data(),
			m_blockSize(rhs.m_blockSize),
			m_allocator(std::move(rhs.m_allocator))
		{
			VX_REGISTER_ANALYZER;

			if (rhs.m_blockSize != 0)
			{
				m_begin = rhs.m_begin;
				m_end = rhs.m_end;
				m_last = rhs.m_last;
			}
			else
			{
				auto otherSize = rhs.size();
				m_end = m_begin + otherSize;

				::memcpy(m_data, rhs.m_data, BUFFER_SIZE);
				::memset(rhs.m_data, 0xd, BUFFER_SIZE);
			}

			rhs.m_begin = reinterpret_cast<pointer>(rhs.m_data);
			rhs.m_end = reinterpret_cast<pointer>(rhs.m_data);
			rhs.m_last = reinterpret_cast<pointer>(rhs.m_data + BUFFER_SIZE);
			rhs.m_blockSize = 0;
#ifdef _VX_ARRAY_ANALYZER
			std::swap(m_arrayStats, rhs.m_arrayStats);
#endif
		}

		~InplaceArray()
		{
#ifdef _VX_ARRAY_ANALYZER
			if (m_blockSize != 0)
			{
				auto &data = getArrayStats();
				data.m_size = static_cast<u32>(size());
				data.m_capacity = static_cast<u32>(capacity());
				vx::ArrayAnalyzer::addData(data);
			}
#endif

			release();
		}

		InplaceArray& operator=(const InplaceArray &rhs)
		{
			if (this != &rhs)
			{
				this->clear();
				this->push_back_range(rhs.begin(), rhs.end());

#ifdef _VX_ARRAY_ANALYZER
				++m_arrayStats.m_copyCount;
#endif
			}
			return *this;
		}

		template<size_t OtherCount, typename OtherAlloc>
		InplaceArray& operator=(const InplaceArray<T, OtherCount, OtherAlloc> &rhs)
		{
			if (this != &rhs)
			{
				this->clear();
				this->push_back_range(rhs.begin(), rhs.end());

#ifdef _VX_ARRAY_ANALYZER
				++m_arrayStats.m_copyCount;
#endif
			}
			return *this;
		}

		InplaceArray& operator=(InplaceArray &&rhs)
		{
			if (this != &rhs)
			{
				swap(rhs);
			}
			return *this;
		}

		void swap(InplaceArray &rhs)
		{
			if (m_blockSize == 0 && rhs.m_blockSize == 0)
			{
				auto thisSize = size();
				auto otherSize = rhs.size();

				u8 tmpBuffer[BUFFER_SIZE];
				::memcpy(tmpBuffer, m_data, BUFFER_SIZE);

				::memcpy(m_data, rhs.m_data, BUFFER_SIZE);
				::memcpy(rhs.m_data, tmpBuffer, BUFFER_SIZE);

				m_end = m_begin + otherSize;
				rhs.m_end = rhs.m_begin + thisSize;
			}
			else if (m_blockSize == 0 && rhs.m_blockSize != 0)
			{
				auto thisSize = size();

				::memcpy(rhs.m_data, m_data, BUFFER_SIZE);
				::memset(m_data, 0xd, BUFFER_SIZE);

				m_begin = rhs.m_begin;
				m_end = rhs.m_end;
				m_last = rhs.m_last;

				rhs.m_begin = reinterpret_cast<pointer>(rhs.m_data);
				rhs.m_end = rhs.m_begin + thisSize;
				rhs.m_last = reinterpret_cast<pointer>(rhs.m_data + BUFFER_SIZE);
			}
			else if (m_blockSize != 0 && rhs.m_blockSize == 0)
			{
				auto otherSize = rhs.size();

				::memcpy(m_data, rhs.m_data, BUFFER_SIZE);
				::memset(rhs.m_data, 0xd, BUFFER_SIZE);

				rhs.m_begin = m_begin;
				rhs.m_end = m_end;
				rhs.m_last = m_last;

				m_begin = reinterpret_cast<pointer>(m_data);
				m_end = m_begin + otherSize;
				m_last = reinterpret_cast<pointer>(m_data + BUFFER_SIZE);
			}
			else
			{
				MyBase::swap(rhs);
			}

			std::swap(m_blockSize, rhs.m_blockSize);
			m_allocator.swap(rhs.m_allocator);
#ifdef _VX_ARRAY_ANALYZER
			std::swap(m_arrayStats, rhs.m_arrayStats);

			++m_arrayStats.m_swapCount;
			++rhs.m_arrayStats.m_swapCount;
#endif
		}

		void release()
		{
			if (m_begin)
			{
				clear();

				if (m_blockSize != 0)
				{
					m_allocator.deallocate({ reinterpret_cast<u8*>(m_begin), m_blockSize });
				}

				m_begin = reinterpret_cast<pointer>(m_data);
				m_end = reinterpret_cast<pointer>(m_data);
				m_last = reinterpret_cast<pointer>(m_data + BUFFER_SIZE);
			}
		}

		void resize(size_t sz)
		{
			auto currentSize = size();
			if (currentSize >= sz)
				return;

			reserve(sz);

			auto d = sz - currentSize;
			while (d)
			{
				push_back(value_type{});
				--d;
			}
		}

		void reserve(size_t c)
		{
			auto currentCapacity = capacity();
			if (c <= currentCapacity)
			{
#ifdef _VX_ARRAY_ANALYZER
				++m_arrayStats.m_reserveLessCount;
#endif
				return;
			}

#ifdef _VX_ARRAY_ANALYZER
			if (m_arrayStats.m_firstReserveCount == 0)
			{
				m_arrayStats.m_firstReserveCount = static_cast<u32>(c);
			}
#endif

			auto size_ = size();
			auto blockPtr = (m_blockSize==0)?nullptr :reinterpret_cast<u8*>(m_begin);

			auto newBlock = m_allocator.reallocate({ blockPtr, m_blockSize }, sizeof(value_type) * c, __alignof(value_type));
			if (blockPtr == nullptr)
			{
				::memcpy(newBlock.ptr, m_data, sizeof(value_type) * size_);
				::memset(m_data, 0xd, BUFFER_SIZE);
			}

			m_begin = reinterpret_cast<pointer>(newBlock.ptr);
			m_last = reinterpret_cast<pointer>(newBlock.ptr + newBlock.size);
			m_end = reinterpret_cast<pointer>(newBlock.ptr) + size_;
			m_blockSize = newBlock.size;

#ifdef _VX_ARRAY_ANALYZER
			++m_arrayStats.m_reserveCount;
			m_arrayStats.updateMaxCapacity(static_cast<u32>(capacity()));
#endif
		}

		void push_back(const value_type &value)
		{
			if (m_end >= m_last)
			{
				reserve(capacity() * 2);
			}

			new (m_end++) value_type{ value };

#ifdef _VX_ARRAY_ANALYZER
			m_arrayStats.updateMaxSize(size());
#endif
		}

		template<typename ...Args>
		void push_back(Args&& ...args)
		{
			if (m_end >= m_last)
			{
				reserve(capacity() * 2);
			}

			new (m_end++) value_type{ std::forward<Args>(args)... };

#ifdef _VX_ARRAY_ANALYZER
			m_arrayStats.updateMaxSize(static_cast<u32>(size()));
#endif
		}

		void push_back_range(const_iterator first, const_iterator last)
		{
			auto cap = capacity();
			auto sz = size();
			auto count = last - first;
			auto newSize = static_cast<u32>(sz + count);
			if (newSize >= cap)
			{
				reserve(newSize);
			}

			auto ptr = first;
			while (ptr != last)
			{
				new (m_end++) value_type{ *(ptr++) };
			}

#ifdef _VX_ARRAY_ANALYZER
			m_arrayStats.updateMaxSize(static_cast<u32>(size()));
#endif
		}

		void push_back_range(const InplaceArray &other)
		{
			auto first = other.begin();
			auto last = other.end();

			push_back_range(first, last);
		}

		template<typename U, size_t OtherCount, typename OtherAlloc, typename Cvt>
		void push_back_range(const InplaceArray<U, OtherCount, OtherAlloc> &other, Cvt cvt)
		{
			auto first = other.begin();
			auto last = other.end();

			auto cap = capacity();
			auto sz = size();
			auto count = last - first;
			auto newSize = static_cast<u32>(sz + count);
			if (newSize >= cap)
			{
				reserve(newSize);
			}

			while (first != last)
			{
				new (m_end++) value_type{ cvt(*(first++)) };
			}

			m_size = newSize;

#ifdef _VX_ARRAY_ANALYZER
			m_arrayStats.updateMaxSize(size());
#endif
		}
	};

	namespace detail
	{
		template<typename T, size_t SIZE, typename Allocator>
		struct GetTypeInfo<vx::InplaceArray<T, SIZE, Allocator>>
		{
			static const auto& get()
			{
				static const auto typeInfo
				{
					get_constexpr()
				};
				return typeInfo;
			}

			constexpr static auto get_constexpr()
			{
				return getTypeInfo(concat("vx::InplaceArray<", GetTypeInfo<T>::get_constexpr().m_name, ", ", IntToString<SIZE>::get().data, ", " , GetTypeInfo<Allocator>::get().m_name, ">"),
					sizeof(vx::InplaceArray<T, SIZE, Allocator>),
					__alignof(vx::InplaceArray<T, SIZE, Allocator>));
			}
		};
	}
}
