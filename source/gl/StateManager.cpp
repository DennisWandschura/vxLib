#include <vxLib\gl\StateManager.h>
#include <vxLib/gl/gl.h>
#include <cstring>

namespace vx
{
	namespace gl
	{
		StateManager::StateManager()
			: m_currentFrameBuffer(0),
			m_currentPipeline(0),
			m_currentVao(0),
			m_currentCapabilities(),
			m_bindBuffer()
		{
			m_currentCapabilities.set((U32)Capabilities::Multisample);
			m_currentCapabilities.set((U32)Capabilities::Dither);
		}

		void StateManager::enable(Capabilities cap)
		{
			if (!m_currentCapabilities.get((U32)cap))
			{
				auto glCap = detail::getCapability(cap);
				glEnable(glCap);
				m_currentCapabilities.set((U32)cap);
			}
		}

		void StateManager::disable(Capabilities cap)
		{
			if (m_currentCapabilities.get((U32)cap))
			{
				auto glCap = detail::getCapability(cap);
				glDisable(glCap);
				m_currentCapabilities.clear((U32)cap);
			}
		}

		void StateManager::setClearColor(F32 r, F32 g, F32 b, F32 a)
		{
			if (m_clearColor.x != r || m_clearColor.y != g || m_clearColor.z != b || m_clearColor.w != a)
			{
				m_clearColor.x = r;
				m_clearColor.y = g;
				m_clearColor.z = b;
				m_clearColor.w = a;

				glClearColor(r, g, b, a);
			}
		}

		void StateManager::setViewport(U32 x, U32 y, U32 width, U32 height)
		{
			if (m_viewPort.z != width || m_viewPort.w != height || m_viewPort.x != x || m_viewPort.y != y)
			{
				glViewport(x, y, width, height);
				m_viewPort.x = x;
				m_viewPort.y = y;
				m_viewPort.z = width;
				m_viewPort.w = height;
			}
		}
		void StateManager::bindFrameBuffer(U32 id)
		{
			if (m_currentFrameBuffer != id)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, id);
				m_currentFrameBuffer = id;
			}
		}

		void StateManager::bindVertexArray(U32 id)
		{
			if (m_currentVao != id)
			{
				glBindVertexArray(id);
				m_currentVao = id;
			}
		}

		void StateManager::bindBuffer(U32 target, U32 id)
		{
			auto index = 0u;
			switch (target)
			{
			case(GL_DRAW_INDIRECT_BUFFER) :
				index = 0;
				break;
			case(GL_PIXEL_PACK_BUFFER) :
				index = 1;
				break;
			case(GL_PIXEL_UNPACK_BUFFER) :
				index = 2;
				break;
			case(GL_UNIFORM_BUFFER) :
				index = 3;
				break;
			case(GL_ATOMIC_COUNTER_BUFFER) :
				index = 4;
				break;
			case(GL_ARRAY_BUFFER) :
				index = 5;
				break;
			case(GL_COPY_READ_BUFFER) :
				index = 6;
				break;
			case(GL_COPY_WRITE_BUFFER) :
				index = 7;
				break;
			case(GL_DISPATCH_INDIRECT_BUFFER) :
				index = 8;
				break;
			case(GL_ELEMENT_ARRAY_BUFFER) :
				index = 9;
				break;
			case(GL_QUERY_BUFFER) :
				index = 10;
				break;
			case(GL_SHADER_STORAGE_BUFFER) :
				index = 11;
				break;
			case(GL_TEXTURE_BUFFER) :
				index = 12;
				break;
			case(GL_TRANSFORM_FEEDBACK_BUFFER) :
				index = 13;
				break;
			default:
				return;
			}

			if (m_bindBuffer[index] != id)
			{
				glBindBuffer(target, id);
				m_bindBuffer[index] = id;
			}
		}

		void StateManager::bindPipeline(U32 pipeline)
		{
			if (m_currentPipeline != pipeline)
			{
				glBindProgramPipeline(pipeline);
				m_currentPipeline = pipeline;
			}
		}
	}
}