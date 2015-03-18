#pragma once

#include <vxLib/gl/SharedObject.h>

namespace vx
{
	namespace gl
	{
		class VertexArray : public SharedObject < VertexArray >
		{
			using MyBase = SharedObject < VertexArray >;

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