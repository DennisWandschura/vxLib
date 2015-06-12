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

#include <thread>

namespace vx
{
	class thread
	{
		std::thread m_thread;

	public:
		thread()
		{

		}

		template<class _Fn,
		class... _Args>
			explicit thread(_Fn&& _Fx, _Args&&... _Ax)
			:m_thread(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...)
		{

		}

		thread(const thread&) = delete;

		thread(thread &&rhs)
			:m_thread(std::move(rhs.m_thread))
		{

		}

		~thread()
		{
			if (m_thread.joinable())
				m_thread.join();
		}

		thread& operator=(const thread&) = delete;

		thread& operator=(thread &&rhs)
		{
			if (this != &rhs)
			{
				m_thread = std::move(rhs.m_thread);
			}
		}

		void swap(thread &other)
		{
			m_thread.swap(other.m_thread);
		}

		void join()
		{
			m_thread.join();
		}

		bool isJoinable() const
		{
			return m_thread.joinable();
		}

		void detach()
		{
			m_thread.detach();
		}
	};
}