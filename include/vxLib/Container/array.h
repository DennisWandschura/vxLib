#ifndef __VX_ARRAY_H
#define __VX_ARRAY_H
#pragma once

#include <vxLib/Container/iterator.h>
#include <vxLib/Allocator/Allocator.h>

namespace vx
{
	template<typename T>
	class array
	{
	public:
		using _MyContainer = array<T>;
		using value_type = T;
		using reference = value_type&;
		using const_reference = const reference;
		using pointer = value_type*;
		using const_pointer = const pointer;

		using const_iterator = vx::vector_const_iterator<_MyContainer>;
		using iterator = vx::vector_iterator<_MyContainer>;

		using size_type = U32;
		using difference_type = size_t;

	private:
		pointer m_pValues{ nullptr };
		size_type m_size{ 0 };
		size_type m_capacity{ 0 };

	public:
		// destroys all values and sets everything to zero
		pointer release()
		{
			auto p = m_pValues;
			if (m_pValues)
			{
				AllocatorBase::rangeDestroy(m_pValues, m_pValues + m_size);
				m_size = 0;
				m_pValues = nullptr;
				m_capacity = 0;
			}
			return p;
		}

		array(){}

		template<typename Alloc>
		array(size_type capacity, Alloc* pAllocator)
			:m_capacity(capacity)
		{
			m_pValues = (pointer)pAllocator->allocate(sizeof(value_type) * capacity, __alignof(value_type));
			VX_ASSERT(m_pValues);
		}

		array(const array&) = delete;

		array(array &&rhs)
			:m_pValues(rhs.m_pValues),
			m_size(rhs.m_size),
			m_capacity(rhs.m_capacity)
		{
			rhs.m_pValues = nullptr;
			rhs.m_size = 0;
			rhs.m_capacity = 0;
		}

		array& operator=(const array&) = delete;

		array& operator=(array &&rhs)
		{
			if (this != &rhs)
			{
				this->swap(rhs);
			}
			return *this;
		}

		~array()
		{
			release();
		}

		void swap(array &other)
		{
			std::swap(m_pValues, other.m_pValues);
			std::swap(m_size, other.m_size);
			std::swap(m_capacity, other.m_capacity);
		}

		void push_back(const value_type &value)
		{
			VX_ASSERT(m_size < m_capacity);
			AllocatorBase::construct(m_pValues + m_size, value);
			++m_size;
		}

		template<typename = typename std::enable_if_t<std::is_nothrow_move_constructible<value_type>::value>>
		void push_back(value_type &&value)
		{
			VX_ASSERT(m_size < m_capacity);
			AllocatorBase::construct(m_pValues + m_size, std::forward<value_type>(value));
			++m_size;
		}

		void pop_back()
		{
			VX_ASSERT(m_size > 0);
			vx::AllocatorBase::destroy(&back());
			--m_size;
		}

		template<typename... _Valty>
		void emplace_back(_Valty&&... _Val)
		{
			VX_ASSERT(m_size < m_capacity);
			AllocatorBase::construct(m_pValues + m_size, std::forward<_Valty>(_Val)...);
			++m_size;
		}

		void clear()
		{
			AllocatorBase::rangeDestroy(m_pValues, m_pValues + m_size);
			m_size = 0;
		}

		reference front()
		{
			return *m_pValues;
		}

		const_reference front() const
		{
			return *m_pValues;
		}

		reference back()
		{
			return *(end() - 1);
		}

		const_reference back() const
		{
			return *(end() - 1);
		}

		reference operator[](size_type i)
		{
			return m_pValues[i];
		}

		const_reference operator[](size_type i) const
		{
			return m_pValues[i];
		}

		const_iterator begin() const
		{
			return const_iterator(m_pValues, this);
		}

		iterator begin()
		{
			return iterator(m_pValues, this);
		}

		const_iterator end() const
		{
			return const_iterator(m_pValues + m_size, this);
		}

		iterator end()
		{
			return iterator(m_pValues + m_size, this);
		}

		pointer data()
		{
			return m_pValues;
		}

		const_pointer data() const
		{
			return m_pValues;
		}

		size_type size() const
		{
			return m_size;
		}
	};
}

#endif