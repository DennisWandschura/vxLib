#pragma once

#include <vxLib/types.h>
#include <atomic>

namespace vx
{
	class spinlock
	{
	protected:
		std::atomic_int m_flag;

	public:
		spinlock()
		{
			m_flag.store(0);
		}

		void lock()
		{
			while (m_flag.exchange(1, std::memory_order_acquire) != 0)
			{
			}
		}

		void unlock()
		{
			m_flag.store(0, std::memory_order_release);
		}
	};
}