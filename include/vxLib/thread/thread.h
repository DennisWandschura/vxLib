#pragma once

#include <vxLib/types.h>
#include <Windows.h>

namespace vx
{
	typedef DWORD(WINAPI *ThreadProcSignature)(void*);

	class thread
	{
		HANDLE m_handle;
		DWORD m_tid;

	public:
		thread() : m_handle(nullptr), m_tid(0) {}

		~thread()
		{
			if (m_handle != nullptr)
			{
				CloseHandle(m_handle);
				m_handle = nullptr;
			}
		}

		bool create(ThreadProcSignature tp, void* param)
		{
			const auto stackSize = 1 MBYTE;

			DWORD tid;
			auto handle = CreateThread(nullptr, stackSize, tp, param, THREAD_SUSPEND_RESUME, &tid);
			if (handle != nullptr)
			{
				m_handle = handle;
				m_tid = tid;
				return true;
			}

			return false;
		}

		void resume()
		{
			if (m_handle)
			{
				ResumeThread(m_handle);
			}
		}

		void suspend()
		{
			if (m_handle)
			{
				SuspendThread(m_handle);
			}
		}
	};
}