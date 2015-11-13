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

#include <vxLib/types.h>
#include <vxLib/type_traits.h>

namespace vx
{
	template<typename T>
	struct NoCheck
	{
		static void check(bool b)
		{
		}
	};

	template<typename T>
	struct AssertCheck
	{
		static void check(bool b)
		{
			if (!b)
			{
				VX_ASSERT(false);
			}
		}
	};

	template<typename T>
	struct CreationExplicit
	{
		static void create()
		{

		}
	};

	template<typename T>
	struct CreationImplicit
	{
		static void create()
		{
			T::create();
		}
	};

	template<typename T, template <class> class CheckingPolicy = NoCheck, template<class> class CreationPolicy = CreationExplicit>
	class GlobalSingleton : public CheckingPolicy < GlobalSingleton<T, CheckingPolicy, CreationPolicy> >, public CreationPolicy< GlobalSingleton<T, CheckingPolicy, CreationPolicy> >
	{
		typedef CheckingPolicy < GlobalSingleton<T, CheckingPolicy, CreationPolicy> > MyCheck;
		typedef CreationPolicy< GlobalSingleton<T, CheckingPolicy, CreationPolicy> > MyCreate;

		struct Data
		{
			union
			{
				u8 m_buffer[sizeof(T)];
				typename detail::SelectAlign<T>::type _aligner;
			};
		};

		static Data s_data;
		static u8 s_constructed;

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

		static void create()
		{
			if (s_constructed == 0)
			{
				::new ((void*)s_data.m_buffer)T{};
				s_constructed = 1;
			}
		}

		template<typename ...Args>
		static void create(Args&& ...args)
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
			//MyCreate::create();
			//MyCheck::check(s_constructed != 0);
			//return *((T*)s_data.m_buffer);
			auto data = reinterpret_cast<T*>(s_data.m_buffer);
			return *data;
		}
	};

	template<typename U, template <class> class CheckingPolicy, template<class> class CreationPolicy>
	typename GlobalSingleton<U, CheckingPolicy, CreationPolicy>::Data GlobalSingleton<U, CheckingPolicy, CreationPolicy>::s_data = {};

	template<class U, template <class> class CheckingPolicy, template<class> class CreationPolicy>
	u8 GlobalSingleton<U, CheckingPolicy, CreationPolicy>::s_constructed{ 0 };
}