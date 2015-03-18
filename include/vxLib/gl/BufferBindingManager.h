#ifndef __VX_GL_BUFFERBINDINGMANAGER_H
#define __VX_GL_BUFFERBINDINGMANAGER_H
#pragma once

#include <vxLib\types.h>
#include <vxLib\Container\sorted_vector.h>

namespace vx
{
	namespace gl
	{
		class BufferBindingManager
		{
			struct Binding
			{
				U32 m_target;
				U32 m_index;

				Binding() :m_target(0), m_index(0){}
				Binding(U32 target, U32 index) :m_target(target), m_index(index){}

				const bool operator<(const Binding &rhs) const
				{
					return (m_target < rhs.m_target) || (m_target == rhs.m_target && m_index < rhs.m_index);
				}
			};

			vx::sorted_vector<Binding, U32> m_bindings;
			U64 m_changedBindings;

		public:
			BufferBindingManager();

			void bind(U32 target, U32 index, U32 bufferId);
		};
	}
}
#endif