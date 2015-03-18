#ifndef __VX_GL_PROGRAMPIPELINE_H
#define __VX_GL_PROGRAMPIPELINE_H
#pragma once

#include <vxLib\gl\Base.h>

namespace vx
{
	namespace gl
	{
		class ShaderProgram;

		class ProgramPipeline : public Base < ProgramPipeline >
		{
			U32 m_programIds[6];

		public:
			ProgramPipeline() noexcept;
			ProgramPipeline(ProgramPipeline &&rhs) noexcept;
			~ProgramPipeline();

			ProgramPipeline& operator=(ProgramPipeline &&rhs) noexcept;

			void create();
			void destroy();

			void useProgram(const ShaderProgram &program);
			void validatePipeline() const;

			void bind() const;

			void bindZero() const;

			U32 getVertexShader() const noexcept;
			U32 getFragmentShader() const noexcept;
			U32 getGeometryShader() const noexcept;
			U32 getComputeShader() const noexcept;
			U32 getTessControlShader() const noexcept;
			U32 getTessEvalShader() const noexcept;
		};
	}
}
#endif