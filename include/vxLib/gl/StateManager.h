#ifndef __VX_GL_STATE_MANAGER_H
#define __VX_GL_STATE_MANAGER_H
#pragma once

#include <vxLib/math/Vector.h>
#include <vxLib\Container\bitset.h>
#include <vxLib\gl\Base.h>

namespace vx
{
	namespace gl
	{
		class StateManager
		{
			static const U32 s_bufferTypeCount = 14;

			U32 m_currentFrameBuffer;
			U32 m_currentPipeline;
			U32 m_currentVao;
			vx::ushort4 m_viewPort{0, 0, 0, 0};
			vx::bitset<32> m_currentCapabilities;
			U32 m_bindBuffer[s_bufferTypeCount];
			vx::float4 m_clearColor{ 1.0f, 1.0f, 1.0f, 1.0f };

		public:
			StateManager();

			void enable(Capabilities cap);
			void disable(Capabilities cap);
			void setClearColor(F32 r, F32 g, F32 b, F32 a);
			void setViewport(U32 x, U32 y, U32 width, U32 height);
			void bindFrameBuffer(U32 id);
			void bindVertexArray(U32 id);
			void bindBuffer(U32 target, U32 id);
			void bindPipeline(U32 pipeline);
		};
	}
}
#endif