#ifndef __VX_GL_STATE_MANAGER_H
#define __VX_GL_STATE_MANAGER_H
#pragma once

#include <vxLib/math/Vector.h>
#include <vxLib/Container/bitset.h>
#include <vxLib/gl/Buffer.h>

namespace vx
{
	namespace gl
	{
		class VertexArray;
		class ProgramPipeline;
		class Framebuffer;

		class StateManager
		{
			static const U32 s_bufferTypeCount = 15;

			static U32 s_currentFrameBuffer;
			static U32 s_currentPipeline;
			static U32 s_currentVao;
			static vx::ushort4 s_viewPort;
			static vx::bitset<32> s_currentCapabilities;
			static U32 s_bindBuffer[s_bufferTypeCount];
			static vx::float4 s_clearColor;

			StateManager();

		public:
			static void enable(Capabilities cap);
			static void disable(Capabilities cap);
			static void setClearColor(F32 r, F32 g, F32 b, F32 a);
			static void setViewport(U32 x, U32 y, U32 width, U32 height);
			static void bindFrameBuffer(U32 id);
			static void bindFrameBuffer(const Framebuffer &fbo);
			static void bindVertexArray(U32 id);
			static void bindVertexArray(const VertexArray &vao);
			static void bindBuffer(BufferType target, U32 id);
			static void bindPipeline(U32 pipeline);
			static void bindPipeline(const ProgramPipeline &pipe);
		};
	}
}
#endif