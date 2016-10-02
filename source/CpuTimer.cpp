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

#include <vxlib/CpuTimer.h>
#ifdef  _VX_PLATFORM_ANDROID
#include <time.h>
#else
#include <Windows.h>
#endif

namespace vx
{
	namespace CpuTimerCpp
	{
		u64 queryPerformanceCounter()
		{
			u64 ticks;
#ifdef  _VX_PLATFORM_ANDROID
			struct timespec now;
			clock_gettime(CLOCK_MONOTONIC, &now);
			ticks = now.tv_sec;
			ticks *= 1000000000;
			ticks += now.tv_nsec;
#else
			QueryPerformanceCounter((LARGE_INTEGER*)&ticks);
#endif
			return ticks;
		}

		u64 queryPerformanceFrequency()
		{
#ifdef  _VX_PLATFORM_ANDROID
			return 1000000000llu;
#else
			u64 frequency;
			QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
			return frequency;
#endif
		}
	}

	u64 CpuTimer::s_frequency{ 0 };

	CpuTimer::CpuTimer()
		:m_start(CpuTimerCpp::queryPerformanceCounter())
	{
		if (s_frequency == 0)
		{
			s_frequency = CpuTimerCpp::queryPerformanceFrequency();
		}
	}

	CpuTimer::~CpuTimer()
	{

	}

	void CpuTimer::reset()
	{
		m_start = CpuTimerCpp::queryPerformanceCounter();
	}

	f64 CpuTimer::getTimeSeconds() const
	{
		auto end = CpuTimerCpp::queryPerformanceCounter();

		f64 time = (end - m_start) * 1000000.0 / s_frequency;

		return time * 1.0e-6;
	}

	f64 CpuTimer::getTimeMiliseconds() const
	{
		auto end = CpuTimerCpp::queryPerformanceCounter();

		f64 time = (end - m_start) * 1000000.0 / s_frequency;

		return (time * 0.001);
	}

	f64 CpuTimer::getTimeMicroseconds() const
	{
		auto end = CpuTimerCpp::queryPerformanceCounter();

		f64 time = (end - m_start) * 1000000.0 / s_frequency;

		return time;
	}
}