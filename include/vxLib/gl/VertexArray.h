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
#ifndef __VX_GL_VERTEXARRAY_H
#define __VX_GL_VERTEXARRAY_H
#pragma once

#include <vxLib/gl/Base.h>

namespace vx
{
	namespace gl
	{
		class VertexArray : public Base < VertexArray >
		{
			using MyBase = Base < VertexArray >;

		public:
			VertexArray();
			VertexArray(VertexArray &&rhs);

			~VertexArray();

			VertexArray& operator=(VertexArray &&rhs);

			void create();

			void destroy();

			void bind();

			void bindZero();

			void enableArrayAttrib(U32 index);
			void arrayAttribBinding(U32 attribIndex, U32 bindingIndex);
			void arrayAttribFormatI(U32 attribIndex, U32 size, DataType type, U32 relativeOffset);
			void arrayAttribFormatF(U32 attribIndex, U32 size, U8 normalized, U32 relativeOffset);
			void arrayBindingDivisor(U32 bindingIndex, U32 divisor);

			template<typename IBO>
			void bindIndexBuffer(const IBO &ibo)
			{
				bindIndexBuffer(ibo.getId());
			}

			void bindIndexBuffer(U32 ibo);

			template<typename VBO>
			void bindVertexBuffer(const VBO &vbo, U32 bindingIndex, size_t offset, size_t stride)
			{
				bindVertexBuffer(vbo.getId(), bindingIndex, offset, stride);
			}

			void bindVertexBuffer(U32 vbo, U32 bindingIndex, size_t offset, size_t stride);
		};
	}
}
#endif