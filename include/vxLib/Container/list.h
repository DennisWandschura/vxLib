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
#include <vxLib/algorithm.h>

namespace vx
{
	namespace detail
	{
		struct NodeBase
		{
			NodeBase* m_next;
			NodeBase* m_prev;

			NodeBase() : m_next(nullptr), m_prev(nullptr) {}
			NodeBase(const NodeBase& rhs) : m_next(rhs.m_next), m_prev(rhs.m_prev) {}
			NodeBase(NodeBase &&rhs) :m_next(rhs.m_next), m_prev(rhs.m_prev) { rhs.m_next = nullptr;rhs.m_prev = nullptr; }

			NodeBase& operator=(const NodeBase&) = delete;
			NodeBase& operator=(NodeBase &&rhs)
			{
				if (this != &rhs)
				{
					swap(rhs);
				}
				return *this;
			}

			void swap(NodeBase &rhs)
			{
				std::swap(m_next, rhs.m_next);
				std::swap(m_prev, rhs.m_prev);
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
			vx::destruct(node);

			AllocatedBlock block{ (u8*)node, sizeof(MyNode) };
			m_allocator.deallocate(block);
		}

		inline MyNode* getLastNode()
		{
			return (MyNode*)m_root.m_prev;
		}

		inline MyNode* getFirstNode()
		{
			return (MyNode*)m_root.m_next;
		}

		void swapRoot(list &rhs)
		{
			auto thisEmpty = empty();
			auto otherEmpty = rhs.empty();

			auto oldRoot = m_root;
			if (!otherEmpty)
			{
				m_root.m_next = rhs.m_root.m_next;
				m_root.m_prev = rhs.m_root.m_prev;
			}

			if (!thisEmpty)
			{
				rhs.m_root.m_next = oldRoot.m_next;
				rhs.m_root.m_prev = oldRoot.m_prev;
			}
			else
			{
				rhs.resetRoot();
			}
		}

		void resetRoot()
		{
			m_root.m_next = &m_root;
			m_root.m_prev = &m_root;
		}

	public:
		list() :m_root(), m_size(0), m_allocator() 
		{
			resetRoot();
		}

		explicit list(Allocator &&allocator) :m_root(), m_size(0), m_allocator(std::move(allocator)) 
		{
			resetRoot();
		}

		template<typename ...Args>
		explicit list(Args&& ...args) : m_root(), m_size(0), m_allocator(std::forward<Args>(args)...) 
		{
			resetRoot();
		}

		list(const list&) = delete;

		list(list &&other)
			: m_root(),
			m_size(other.m_size),
			m_allocator(std::move(other.m_allocator))
		{
			swapRoot(other);
		}

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
			swapRoot(other);
			std::swap(m_size, other.m_size);
			m_allocator.swap(other.m_allocator);
		}

		void clear()
		{
			auto node = begin();
			auto endNode = end();

			while (node != endNode)
			{
				auto p = node;
				node = (MyNode*)p->m_next;

				deleteNode(p);
			}

			resetRoot();
		}

		template <class... Args>
		bool push_back(Args&& ...args)
		{
			auto node = createNode(std::forward<Args>(args)...);
			if (node == nullptr)
				return false;

			auto prevNode = getLastNode();
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
			auto nodeToDelete = getLastNode();
			auto prevNode = nodeToDelete->m_prev;

			deleteNode((MyNode*)nodeToDelete);

			m_root.m_prev = prevNode;
			prevNode->m_next = &m_root;
		}

		MyNode* erase(MyNode* iter)
		{
			auto prevNode = iter->m_prev;
			auto nextNode = iter->m_next;

			prevNode->m_next = nextNode;
			nextNode->m_prev = prevNode;

			deleteNode(iter);

			--m_size;

			return (MyNode*)nextNode;
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
			return getLastNode()->m_data;
		}

		inline const value_type& back() const
		{
			return getLastNode()->m_data;
		}

		inline MyNode* begin() { return getFirstNode(); }
		inline const MyNode* begin() const { return getFirstNode(); }

		inline MyNode* end() { return (MyNode*)&m_root; }
		inline const MyNode* end() const { return (MyNode*)&m_root; }

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