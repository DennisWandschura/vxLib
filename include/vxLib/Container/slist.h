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

#include <vxLib/Allocator/Allocator.h>
#include <algorithm>

namespace vx
{
	template<typename T>
	struct ListNodeSingleLinked
	{
		T m_data;
		ListNodeSingleLinked* m_next;

		template <class... Args>
		ListNodeSingleLinked(Args&& ...args) : m_data(std::forward<Args>(args)...), m_next(nullptr) {}

		ListNodeSingleLinked(const ListNodeSingleLinked&) = delete;

		ListNodeSingleLinked(ListNodeSingleLinked &&rhs)
			:m_data(std::move(rhs.m_data)),
			m_next(rhs.m_next)
		{
			rhs.m_next = nullptr;
		}

		~ListNodeSingleLinked() {}

		ListNodeSingleLinked& operator=(const ListNodeSingleLinked&) = delete;

		ListNodeSingleLinked& operator=(ListNodeSingleLinked &&rhs)
		{
			if (this != &rhs)
			{
				m_data = std::move(rhs.m_data);
				m_next = rhs.m_next;

				rhs.m_next = nullptr;
			}
			return *this;
		}

		ListNodeSingleLinked* next()
		{
			return m_next;
		}

		const ListNodeSingleLinked* next() const
		{
			return m_next;
		}
	};

	template<typename T, typename Allocator>
	class slist
	{
		typedef T value_type;
		typedef value_type* pointer;
		typedef ListNodeSingleLinked<value_type> MyNode;

		MyNode* m_head;
		MyNode* m_tail;
		size_t m_size;
		Allocator m_allocator;

		template <class... Args>
		MyNode* createNode(Args&& ...args)
		{
			AllocatedBlock block = m_allocator.allocate(sizeof(MyNode), __alignof(MyNode));

			MyNode* node = (MyNode*)block.ptr;
			if (block.ptr != nullptr)
			{
				assert(block.size == sizeof(MyNode));

				new (node) MyNode{ std::forward<Args>(args)... };
			}

			return node;
		}

		void deleteNode(MyNode* node)
		{
			node->~MyNode();

			AllocatedBlock block{ (u8*)node, sizeof(MyNode) };
			m_allocator.deallocate(block);
		}

	public:
		slist() :m_head(nullptr), m_tail(nullptr), m_size(0), m_allocator() {}

		explicit slist(Allocator &&allocator) :m_head(nullptr), m_tail(nullptr), m_size(0), m_allocator(std::move(allocator)) {}

		template<typename T>
		explicit slist(T* alloc) : slist(std::move(Allocator(alloc))) {}

		slist(const slist&) = delete;

		slist(slist &&other)
			: m_head(other.m_head),
			m_tail(other.m_tail),
			m_size(other.m_size),
			m_allocator(std::move(other.m_allocator))
		{
			other.m_head = nullptr;
			other.m_tail = nullptr;
		}

		~slist()
		{
			clear();
		}

		slist& operator=(const slist&) = delete;

		slist& operator=(slist &&rhs)
		{
			if (this != &rhs)
			{
				swap(rhs);
			}
			return *this;
		}

		void swap(slist &other)
		{
			std::swap(m_head, other.m_head);
			std::swap(m_tail, other.m_tail);
			std::swap(m_size, other.m_size);
			m_allocator.swap(other.m_allocator);
		}

		void clear()
		{
			auto node = m_head;

			while (node != nullptr)
			{
				auto p = node;
				node = p->m_next;

				deleteNode(p);
			}

			m_head = m_tail = nullptr;
		}

		template <class... Args>
		bool push_back(Args&& ...args)
		{
			auto node = createNode(std::forward<Args>(args)...);
			if (node == nullptr)
				return false;

			if (m_head)
			{
				m_tail->m_next = node;
				m_tail = node;
			}
			else
			{
				m_head = m_tail = node;
			}

			++m_size;
			return true;
		}

		void pop_front()
		{
			auto nodeToDelete = m_head;

			auto nextNode = nodeToDelete->m_next;

			deleteNode(nodeToDelete);

			m_head = nextNode;

			--m_size;
		}

		inline T& front()
		{
			return m_head->m_data;
		}

		inline const T& front() const
		{
			return m_head->m_data;
		}

		inline MyNode* begin()
		{
			return m_head;
		}

		inline MyNode* end()
		{
			return nullptr;
		}

		inline const MyNode* begin() const
		{
			return m_head;
		}

		inline const MyNode* end() const
		{
			return nullptr;
		}

		size_t size() const
		{
			return m_size;
		}

		bool empty() const
		{
			return (m_size == 0);
		}
	};
}