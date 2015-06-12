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

#include <atomic>
#include <vxLib/thread/Semaphore.h>

namespace vx
{
	class AutoResetEvent
	{
		// m_status == 1: Event object is signaled.
		// m_status == 0: Event object is reset and no threads are waiting.
		// m_status == -N: Event object is reset and N threads are waiting.
		std::atomic<int> m_status;
		Semaphore m_sema;

	public:
		AutoResetEvent()
			:m_status(0),
			m_sema()
		{

		}

		void wait()
		{
			auto oldStatus = m_status.fetch_sub(1);
			if (oldStatus < 1)
			{
				m_sema.wait();
			}
		}

		void signal()
		{
			int oldStatus = m_status.load(std::memory_order_relaxed);
			for (;;)    // Increment m_status atomically via CAS loop.
			{
				assert(oldStatus <= 1);
				int newStatus = oldStatus < 1 ? oldStatus + 1 : 1;
				if (m_status.compare_exchange_weak(oldStatus, newStatus, std::memory_order_release, std::memory_order_relaxed))
					break;
				// The compare-exchange failed, likely because another thread changed m_status.
				// oldStatus has been updated. Retry the CAS loop.
			}
			if (oldStatus < 0)
				m_sema.signal();    // Release one waiting thread.
		}
	};
}