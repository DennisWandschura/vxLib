#include <vxLib\gl\BufferBindingManager.h>
#include <vxLib/gl/gl.h>

namespace vx
{
	namespace gl
	{
		BufferBindingManager::BufferBindingManager()
			:m_bindings(),
			m_changedBindings(0)
		{
		}

		void BufferBindingManager::bind(U32 target, U32 index, U32 bufferId)
		{
			auto binding = Binding(target, index);

			auto it = m_bindings.find(binding);

			// case new binding
			if (it == m_bindings.end())
			{
				m_bindings.insert(binding, bufferId);
				glBindBufferBase(target, index, bufferId);
				++m_changedBindings;
				return;
			}

			// case buffer alreay bound and different buffer
			if (*it != bufferId)
			{
				// bind new buffer
				*it = bufferId;
				glBindBufferBase(target, index, bufferId);
				++m_changedBindings;
			}
		}
	}
}