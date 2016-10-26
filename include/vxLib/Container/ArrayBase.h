#pragma once

#include <vxLib/algorithm.h>

namespace vx
{
	template<typename T>
	class ArrayBase
	{
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;

		typedef pointer iterator;
		typedef const_pointer const_iterator;

	protected:
		pointer m_begin;
		pointer m_end;
		pointer m_last;

	public:
		//constexpr ArrayBase() :m_begin(nullptr), m_end(nullptr), m_last(nullptr) {}
		constexpr ArrayBase(pointer begin, pointer last) :m_begin(begin), m_end(begin), m_last(last) {}
		constexpr ArrayBase(pointer begin, pointer end, pointer last) : m_begin(begin), m_end(end), m_last(last) {}

		void swap(ArrayBase &rhs)
		{
			std::swap(m_begin, rhs.m_begin);
			std::swap(m_end, rhs.m_end);
			std::swap(m_last, rhs.m_last);
		}

		u32 push_back(const value_type &value)
		{
			u32 result = 0;
			if (m_end < m_last)
			{
				new (m_end++) value_type{ value };
				result = 1;
			}
			return result;
		}

		template<typename ...Args>
		u32 push_back(Args&& ...args)
		{
			u32 result = 0;
			if (m_end < m_last)
			{
				new (m_end++) value_type{ std::forward<Args>(args).. };
				result = 1;
			}
			return result;
		}

		void pop_back()
		{
			vx::destruct(--m_end);
		}

		void clear()
		{
			vx::destruct(begin(), end());
			m_end = m_begin;
		}

		pointer erase(pointer p)
		{
			auto bg = begin();
			VX_ASSERT(p >= bg &&
				m_last > p);

			auto idx = p - bg;
			vx::destruct(p);

			vx::move(p + 1, m_end--, p);

			return (bg + idx);
		}

		void sort()
		{
			std::sort(begin(), end());
		}

		template<typename Cmp>
		void sort(Cmp cmp)
		{
			std::sort(begin(), end(), cmp);
		}

		iterator find(const value_type &key)
		{
			return std::find(begin(), end(), key);
		}

		const_iterator find(const value_type &key) const
		{
			return std::find(begin(), end(), key);
		}

		template<typename Cmp>
		iterator find_if(Cmp cmp)
		{
			return std::find_if(begin(), end(), cmp);
		}

		template<typename Cmp>
		const_iterator find_if(Cmp cmp) const
		{
			return std::find_if(begin(), end(), cmp);
		}

		const_iterator find_lower_bound(const value_type &key) const
		{
			return std::lower_bound(begin(), end(), key);
		}

		template<typename Cmp>
		const_iterator find_lower_bound(const value_type &key, Cmp cmp) const
		{
			return std::lower_bound(begin(), end(), key, cmp);
		}

		iterator find_lower_bound(const value_type &key)
		{
			return std::lower_bound(begin(), end(), key);
		}

		template<typename Cmp>
		iterator find_lower_bound(const value_type &key, Cmp cmp)
		{
			return std::lower_bound(begin(), end(), key, cmp);
		}

		reference back()
		{
			return *(m_end - 1);
		}

		const_reference back() const
		{
			return *(m_end - 1);
		}

		reference operator[](size_t index)
		{
			return m_begin[index];
		}

		const_reference operator[](size_t index) const
		{
			return m_begin[index];
		}

		pointer data() { return m_begin; }
		const_pointer data() const { return m_begin; }

		pointer begin() { return m_begin; }
		const_pointer begin() const { return m_begin; }

		pointer end() { return m_end; }
		const_pointer end() const { return m_end; }

		bool empty() const { return (m_end == m_begin); }

		size_t size() const { return (m_end - m_begin); }
		size_t capacity() const { return (m_last - m_begin); }
	};
}