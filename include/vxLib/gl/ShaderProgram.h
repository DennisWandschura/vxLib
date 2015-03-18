#ifndef __VX_GL_SHADERPROGRAM_H
#define __VX_GL_SHADERPROGRAM_H
#pragma once

#include <vxLib/gl/Base.h>
#include <memory>

namespace vx
{
	namespace gl
	{
		class ShaderProgram : public Base < ShaderProgram >
		{
			using MyBase = Base < ShaderProgram >;

			ShaderProgramType m_type;

			bool checkLinkStatus();
			bool checkValidateStatuts();

			std::unique_ptr<char[]> getProgramInfoLog(int *pLength);

		public:
			explicit ShaderProgram(ShaderProgramType type);
			ShaderProgram(ShaderProgram &&rhs) noexcept;
			~ShaderProgram();

			ShaderProgram& operator=(ShaderProgram &&rhs) noexcept;

			// on failure returns error log and logLength including null terminator
			std::unique_ptr<char[]> create(const char **program, I32 &logLength);

			void destroy();

			ShaderProgramType getType() const { return m_type; }
		};
	}
}
#endif