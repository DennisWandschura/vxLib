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
#ifndef __VX_SCOPEGUARD_H
#define __VX_SCOPEGUARD_H
#pragma once

#include <vxLib\types.h>

namespace vx
{
	template<typename Fun>
	class ScopeGuard
	{
		Fun _f;
		bool _active;

	public:
		ScopeGuard(Fun f)
			:_f(std::move(f)),
			_active(true)
		{}

		ScopeGuard() = delete;
		ScopeGuard(const ScopeGuard&) = delete;
		ScopeGuard(ScopeGuard&&rhs)
			:_f(std::move(rhs._f))
			, _active(rhs._active)
		{
			rhs.dismiss();
		}

		~ScopeGuard()
		{
			if (_active)
			{
#if _VX_EXCEPTIONS
				try
				{
					_f();
				}
				catch (...)
				{

				}
#else
				_f();
#endif
			}
		}

		ScopeGuard& operator=(const ScopeGuard&) = delete;

		void dismiss()
		{
			_active = false;
		}
	};

	template<class Fun>
	ScopeGuard<Fun> scopeGuard(Fun f)
	{
		return ScopeGuard<Fun>(std::move(f));
	}

	namespace detail
	{
		enum class ScopeGuardOnExit{};
		template<typename Fun> ScopeGuard<Fun> operator+(ScopeGuardOnExit, Fun &&fn)
		{
			return ScopeGuard<Fun>(std::forward<Fun>(fn));
		}
	}

#ifdef __COUNTER__
#define SCOPE_EXIT \
	auto CONCATENATE(SCOPE_EXIT_STATE, __COUNTER__) \
	= vx::detail::ScopeGuardOnExit() + [&]()
#else
#define SCOPE_EXIT \
	auto CONCATENATE(SCOPE_EXIT_STATE, __LINE__) \
	= vx::detail::ScopeGuardOnExit() + [&]()
#endif
}
#endif