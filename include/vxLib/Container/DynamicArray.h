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
#include <vxLib/TypeInfo.h>
#include <vxLib/ArrayAnalyzer.h>
#include <vxLib/Allocator/Mallocator.h>

namespace vx
{
	template<typename T, typename Allocator = Mallocator>
	class DynamicArray : public ArrayBase<T>
	{
		typedef Allocator MyAllocator;
		typedef ArrayBase<T> MyBase;

		VX_TYPE_INFO;

		u64 m_blockSize;
		MyAllocator m_allocator;
#ifdef _VX_ARRAY_ANALYZER
		ArrayStats m_arrayStats;
#endif

	public:
		typedef typename MyBase::value_type value_type;
		typedef typename MyBase::pointer pointer;
		typedef typename MyBase::const_pointer const_pointer;
		typedef typename MyBase::reference reference;
		typedef typename MyBase::const_reference const_reference;

		typedef typename MyBase::iterator iterator;
		typedef typename MyBase::const_iterator const_iterator;

		DynamicArray() :MyBase(nullptr, nullptr), m_blockSize(0), m_allocator() { VX_REGISTER_ANALYZER; }

		explicit DynamicArray(MyAllocator &&allocator)
			:MyBase(nullptr, nullptr), m_blockSize(0), m_allocator(std::move(allocator))
		{
			VX_REGISTER_ANALYZER;
		}

		DynamicArray(MyAllocator &&allocator, u32 capacity) :MyBase(nullptr, nullptr), m_blockSize(0), m_allocator(std::move(allocator))
		{
			VX_REGISTER_ANALYZER;

			reserve(capacity);
		}

		template<typename OtherAlloc>
		DynamicArray(MyAllocator &&allocator, const DynamicArray<T, OtherAlloc> &rhs)
			: DynamicArray(std::move(allocator), rhs.capacity())
		{
			VX_REGISTER_ANALYZER;

			push_back_range(rhs.begin(), rhs.end());
		}

		DynamicArray(DynamicArray &&rhs)
			: MyBase(std::move(rhs)),
			m_blockSize(rhs.m_blockSize),
			m_allocator(std::move(rhs.m_allocator))
		{
			rhs.m_blockSize = 0;
#ifdef _VX_ARRAY_ANALYZER
			m_arrayStats.swap(rhs.m_arrayStats);
#endif
		}

		~DynamicArray()
		{
#ifdef _VX_ARRAY_ANALYZER
			if (m_begin != m_last)
			{
				auto &data = getArrayStats();
				data.m_size = static_cast<u32>(size());
				data.m_capacity = static_cast<u32>(capacity());
				vx::ArrayAnalyzer::addData(data);
			}
#endif

			release();
		}

		template<typename OtherAlloc>
		DynamicArray& operator=(const DynamicArray<T, OtherAlloc> &rhs)
		{
			if (this != &rhs)
			{
				this->clear();
				this->reserve(rhs.size());

				this->push_back_range(rhs.begin(), rhs.end());
			}
			return *this;
		}

		DynamicArray& operator=(const DynamicArray &rhs)
		{
			if (this != &rhs)
			{
				this->clear();
				this->reserve(rhs.size());

				this->push_back_range(rhs.begin(), rhs.end());
			}
			return *this;
		}

		DynamicArray& operator=(DynamicArray &&rhs)
		{
			if (this != &rhs)
			{
				swap(rhs);
			}
			return *this;
		}

		void swap(DynamicArray &rhs)
		{
			MyBase::swap(rhs);
			std::swap(m_blockSize, rhs.m_blockSize);
			m_allocator.swap(rhs.m_allocator);
#ifdef _VX_ARRAY_ANALYZER
			m_arrayStats.swap(rhs.m_arrayStats);

			++m_arrayStats.m_swapCount;
			++rhs.m_arrayStats.m_swapCount;
#endif
		}

		void release()
		{
			if (m_begin)
			{
				clear();
				m_allocator.deallocate({ reinterpret_cast<u8*>(m_begin), m_blockSize });
				m_begin = nullptr;
				m_end = nullptr;
				m_last = 0;
			}
		}

		void resize(u64 sz)
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

		void reserve(u64 c)
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

			auto oldSize = size();
			auto newBlock = m_allocator.reallocate({ reinterpret_cast<u8*>(m_begin), m_blockSize }, sizeof(value_type) * c, __alignof(value_type));

			m_begin = reinterpret_cast<pointer>(newBlock.ptr);
			m_end = m_begin + oldSize;
			m_last = reinterpret_cast<T*>(newBlock.ptr + newBlock.size);
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
				auto cap = capacity();
				cap = (cap == 0) ? 1 : cap * 2;
				reserve(cap);
			}

			new (m_end++) value_type{ value };

#ifdef _VX_ARRAY_ANALYZER
			m_arrayStats.updateMaxSize(static_cast<u32>(size()));
#endif
		}

		template<typename ...Args>
		void push_back(Args&& ...args)
		{
			if (m_end >= m_last)
			{
				auto cap = capacity();
				cap = (cap == 0) ? 1 : cap * 2;
				reserve(cap);
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

			auto current = first;
			while (current != last)
			{
				new (m_end++) value_type{ *(current++) };
			}

#ifdef _VX_ARRAY_ANALYZER
			m_arrayStats.updateMaxSize(static_cast<u32>(size()));
#endif
		}

		void push_back_range(const DynamicArray &other)
		{
			auto first = other.begin();
			auto last = other.end();

			push_back_range(first, last);
		}

		template<typename U, typename Cvt>
		void push_back_range(const DynamicArray<U> &other, Cvt cvt)
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

#ifdef _VX_ARRAY_ANALYZER
			m_arrayStats.updateMaxSize(static_cast<u32>(size()));
#endif
		}

#ifdef _VX_ARRAY_ANALYZER
		const ArrayStats& getArrayStats() const { return m_arrayStats; }
		ArrayStats& getArrayStats() { return m_arrayStats; }
#endif

	};

	namespace detail
	{
		template<typename T, typename Allocator>
		struct GetTypeInfo<vx::DynamicArray<T, Allocator>>
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
				return getTypeInfo(concat("vx::DynamicArray<",
					GetTypeInfo<T>::get_constexpr().m_name, ", ",
					GetTypeInfo<Allocator>::get_constexpr().m_name, ">"), 
					sizeof(vx::DynamicArray<T, Allocator>),
					__alignof(vx::DynamicArray<T, Allocator>));
			}
		};
	}
}
