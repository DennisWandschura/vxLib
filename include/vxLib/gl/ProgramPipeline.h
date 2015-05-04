/*
The MIT License (MIT)

Copyright (c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
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