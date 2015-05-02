#include <vxLib\gl\StateManager.h>
#include <vxLib/gl/gl.h>
#include <cstring>
#include <vxLib/gl/VertexArray.h>
#include <vxLib/gl/Framebuffer.h>
#include <vxLib/gl/ProgramPipeline.h>

namespace vx
{
	namespace gl
	{
		U32 StateManager::s_currentFrameBuffer{0};
		U32 StateManager::s_currentPipeline{0};
		U32 StateManager::s_currentVao{0};
		vx::ushort4 StateManager::s_viewPort{ 0, 0, 0, 0 };
		vx::bitset<32> StateManager::s_currentCapabilities{};
		U32 StateManager::s_bindBuffer[s_bufferTypeCount]{};
		vx::float4 StateManager::s_clearColor{ 1.0f, 1.0f, 1.0f, 1.0f };

		StateManager::StateManager()
		{
		}

		void StateManager::enable(Capabilities cap)
		{
			if (!s_currentCapabilities.get((U32)cap))
			{
				auto glCap = detail::getCapability(cap);
				glEnable(glCap);
				s_currentCapabilities.set((U32)cap);
			}
		}

		void StateManager::disable(Capabilities cap)
		{
			if (s_currentCapabilities.get((U32)cap))
			{
				auto glCap = detail::getCapability(cap);
				glDisable(glCap);
				s_currentCapabilities.clear((U32)cap);
			}
		}

		void StateManager::setClearColor(F32 r, F32 g, F32 b, F32 a)
		{
			if (s_clearColor.x != r || s_clearColor.y != g || s_clearColor.z != b || s_clearColor.w != a)
			{
				s_clearColor.x = r;
				s_clearColor.y = g;
				s_clearColor.z = b;
				s_clearColor.w = a;

				glClearColor(r, g, b, a);
			}
		}

		void StateManager::setViewport(U32 x, U32 y, U32 width, U32 height)
		{
			if (s_viewPort.z != width || s_viewPort.w != height || s_viewPort.x != x || s_viewPort.y != y)
			{
				glViewport(x, y, width, height);
				s_viewPort.x = x;
				s_viewPort.y = y;
				s_viewPort.z = width;
				s_viewPort.w = height;
			}
		}
		void StateManager::bindFrameBuffer(U32 id)
		{
			if (s_currentFrameBuffer != id)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, id);
				s_currentFrameBuffer = id;
			}
		}

		void StateManager::bindFrameBuffer(const Framebuffer &fbo)
		{
			bindFrameBuffer(fbo.getId());
		}

		void StateManager::bindVertexArray(U32 id)
		{
			if (s_currentVao != id)
			{
				glBindVertexArray(id);
				s_currentVao = id;
			}
		}

		void StateManager::bindVertexArray(const VertexArray &vao)
		{
			bindVertexArray(vao.getId());
		}

		void StateManager::bindBuffer(BufferType target, U32 id)
		{
			U32 index = (U32)target;
			auto glTarget = detail::BufferInterface::getTarget(target);

			if (s_bindBuffer[index] != id)
			{
				glBindBuffer(glTarget, id);
				s_bindBuffer[index] = id;
			}
		}

		void StateManager::bindPipeline(U32 pipeline)
		{
			if (s_currentPipeline != pipeline)
			{
				glBindProgramPipeline(pipeline);
				s_currentPipeline = pipeline;
			}
		}

		void StateManager::bindPipeline(const ProgramPipeline &pipe)
		{
			bindPipeline(pipe.getId());
		}
	}
}