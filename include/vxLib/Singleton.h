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
#ifndef __VX_SINGLETON_H
#define __VX_SINGLETON_H
#pragma once

#include <vxLib/types.h>
#include <vxLib\type_traits.h>
#include <new>

namespace vx
{
	template<class T>
	struct NoCheck
	{
		static void check(bool b)
		{
		}
	};

	template<class T>
	struct AssertCheck
	{
		static void check(bool b)
		{
			if (!b)
			{
				VX_ASSERT(false, "");
			}
		}
	};

	template<class T>
	struct CallbackCheck
	{
		static void check(bool b)
		{
			if (!b)
			{
				T::checkCallback();
			}
		}
	};

	template<class T, template <class> class CheckingPolicy = NoCheck>
	class GlobalSingleton : public CheckingPolicy<GlobalSingleton<T, CheckingPolicy>>
	{
		using MyCheck = CheckingPolicy<GlobalSingleton<T, CheckingPolicy>>;

		struct Data
		{
			union
			{
				U8 m_buffer[sizeof(T)];
				typename detail::SelectAlign<T>::type _aligner;
			};
		};

		static Data s_data;
		static U8 s_constructed;

		GlobalSingleton(){}

		~GlobalSingleton()
		{
			destroy();
		}

		static void checkCallback()
		{
			construct();
		}

		friend MyCheck;

	public:
		GlobalSingleton(const GlobalSingleton&) = delete;
		GlobalSingleton(GlobalSingleton&&) = delete;

		GlobalSingleton& operator=(const GlobalSingleton&) = delete;
		GlobalSingleton& operator=(GlobalSingleton&&) = delete;

		static void construct()
		{
			if (s_constructed == 0)
			{
				::new ((void*)s_data.m_buffer)T{};
				s_constructed = 1;
			}
		}

		template<typename ...Args>
		static void construct(Args&& ...args)
		{
			if (s_constructed == 0)
			{
				::new ((void*)s_data.m_buffer)T(std::forward<Args>(args)...);
				s_constructed = 1;
			}
		}

		static void destroy()
		{
			if (s_constructed != 0)
			{
				get().~T();
				s_constructed = 0;
			}
		}

		static T& get()
		{
			MyCheck::check(s_constructed != 0);
			return *((T*)s_data.m_buffer);
		}
	};

	template<class U, template <class> class CheckingPolicy>
	GlobalSingleton<U, CheckingPolicy>::Data GlobalSingleton<U, CheckingPolicy>::s_data{};

	template<class U, template <class> class CheckingPolicy>
	U8 GlobalSingleton<U, CheckingPolicy>::s_constructed{ 0 };
}

#endif