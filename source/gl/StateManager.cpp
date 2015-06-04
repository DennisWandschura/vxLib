/*
The MIT License(MIT)

Copyright(c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <vxLib/gl/StateManager.h>
#include <vxLib/gl/gl.h>
#include <cstring>
#include <vxLib/gl/VertexArray.h>
#include <vxLib/gl/Framebuffer.h>
#include <vxLib/gl/ProgramPipeline.h>
#include <vxLib/gl/Buffer.h>

namespace vx
{
	namespace gl
	{
		u32 StateManager::s_currentFrameBuffer{0};
		u32 StateManager::s_currentPipeline{0};
		u32 StateManager::s_currentVao{0};
		vx::ushort4 StateManager::s_viewPort{ 0, 0, 0, 0 };
		vx::bitset<32> StateManager::s_currentCapabilities{};
		u32 StateManager::s_bindBuffer[s_bufferTypeCount]{};
		vx::float4 StateManager::s_clearColor{ 1.0f, 1.0f, 1.0f, 1.0f };

		StateManager::StateManager()
		{
		}

		void StateManager::enable(Capabilities cap)
		{
			if (!s_currentCapabilities.get((u32)cap))
			{
				auto glCap = detail::getCapability(cap);
				glEnable(glCap);
				s_currentCapabilities.set((u32)cap);
			}
		}

		void StateManager::disable(Capabilities cap)
		{
			if (s_currentCapabilities.get((u32)cap))
			{
				auto glCap = detail::getCapability(cap);
				glDisable(glCap);
				s_currentCapabilities.clear((u32)cap);
			}
		}

		void StateManager::setClearColor(f32 r, f32 g, f32 b, f32 a)
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

		void StateManager::setViewport(u32 x, u32 y, u32 width, u32 height)
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
		void StateManager::bindFrameBuffer(u32 id)
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

		void StateManager::bindVertexArray(u32 id)
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

		void StateManager::bindBuffer(BufferType target, u32 id)
		{
			u32 index = (u32)target;
			auto glTarget = detail::BufferInterface::getTarget(target);

			if (s_bindBuffer[index] != id)
			{
				glBindBuffer(glTarget, id);
				s_bindBuffer[index] = id;
			}
		}

		void StateManager::bindBuffer(BufferType target, const Buffer &buffer)
		{
			bindBuffer(target, buffer.getId());
		}

		void StateManager::bindPipeline(u32 pipeline)
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