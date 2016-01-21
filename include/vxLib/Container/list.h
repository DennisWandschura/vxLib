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
	namespace detail
	{
		struct NodeBase
		{
			NodeBase* m_next;
			NodeBase* m_prev;

			NodeBase() : m_next(nullptr), m_prev(nullptr) {}
			NodeBase(const NodeBase&) = delete;
			NodeBase(NodeBase &&rhs) : m_next(rhs.m_next), m_prev(rhs.m_prev) { rhs.m_next = nullptr;rhs.m_prev = nullptr; }

			NodeBase& operator=(const NodeBase&) = delete;
			NodeBase& operator=(NodeBase &&rhs)
			{
				if (this != &rhs)
				{
					std::swap(m_next, rhs.m_next);
					std::swap(m_prev, rhs.m_prev);
				}
				return *this;
			}
		};

		template<typename T>
		struct Node : public NodeBase
		{
			T m_data;

			Node() :NodeBase(), k() {}

			template <class... Args>
			Node(Args&& ...args) : NodeBase(), m_data(std::forward<Args>(args)...) {}

			Node(const Node&) = delete;

			Node(Node &&rhs)
				:m_data(std::move(rhs.m_data)),
				m_next(rhs.m_next)
			{
				rhs.m_next = nullptr;
			}

			~Node() {}

			Node& operator=(const Node&) = delete;

			Node& operator=(Node &&rhs)
			{
				Node::operator=(std::move(rhs));

				if (this != &rhs)
				{
					m_data = std::move(rhs.m_data);

					rhs.m_next = nullptr;
				}
				return *this;
			}

			Node* next() { return (Node*)m_next; }
			const Node* next() const { return (Node*)m_next; }
		};
	}

	template<typename T, typename Allocator>
	class list
	{
		typedef T value_type;
		typedef value_type* pointer;
		typedef detail::Node<value_type> MyNode;

		detail::NodeBase m_root;
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
		list() :m_root(), m_size(0), m_allocator() {}

		explicit list(Allocator &&allocator) :m_root(), m_size(0), m_allocator(std::move(allocator)) {}

		template<typename T>
		explicit list(T* alloc) : slist(std::move(Allocator(alloc))) {}

		list(const list&) = delete;

		list(list &&other)
			: m_root(std::move(other.m_root)),
			m_size(other.m_size),
			m_allocator(std::move(other.m_allocator))
		{}

		~list()
		{
			clear();
		}

		list& operator=(const list&) = delete;

		list& operator=(list &&rhs)
		{
			if (this != &rhs)
			{
				swap(rhs);
			}
			return *this;
		}

		void swap(list &other)
		{
			std::swap(m_root, other.m_root);
			std::swap(m_size, other.m_size);
			m_allocator.swap(other.m_allocator);
		}

		void clear()
		{
			auto node = begin();

			while (node != nullptr)
			{
				auto p = node;
				node = p->m_next;

				deleteNode(p);
			}

			m_root.m_next = m_root.m_prev = nullptr;
		}

		template <class... Args>
		bool push_back(Args&& ...args)
		{
			auto node = createNode(std::forward<Args>(args)...);
			if (node == nullptr)
				return false;

			auto prevNode = m_root.m_prev;
			prevNode->m_next = node;

			node->m_prev = prevNode;
			node->m_next = &m_root;

			m_root.m_prev = node;

			++m_size;
			return true;
		}

		void pop_front()
		{
			auto nodeToDelete = begin();
			auto nextNode = nodeToDelete->m_next;

			deleteNode(nodeToDelete);

			nextNode->m_prev = &m_root;
			m_root.m_next = nextNode;

			--m_size;
		}

		void pop_back()
		{
			auto nodeToDelete = m_root.m_prev;
			auto prevNode = nodeToDelete->m_prev;

			deleteNode(nodeToDelete);

			m_root.m_prev = prevNode;
			prevNode->m_next = &m_root;
		}

		inline value_type& front()
		{
			return begin()->m_data;
		}

		inline const value_type& front() const
		{
			return begin()->m_data;
		}

		inline value_type& back()
		{
			return m_root.m_prev->m_data;
		}

		inline const value_type& back() const
		{
			return m_root.m_prev->m_data;
		}

		inline MyNode* begin() { return m_root.m_next; }
		inline const MyNode* begin() const { return m_root.m_next; }

		inline MyNode* end() { return (Node*)&m_root; }
		inline const MyNode* end() const { return (Node*)&m_root; }

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