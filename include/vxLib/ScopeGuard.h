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
				try
				{
					_f();
				}
				catch (...)
				{

				}
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