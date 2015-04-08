#include <vxLib\gl\VertexArray.h>
#include <vxLib\gl\gl.h>

namespace vx
{
	namespace gl
	{
		VertexArray::VertexArray():MyBase(){}

		VertexArray::VertexArray(VertexArray &&rhs)
			: MyBase(std::move(rhs))
		{
		}

		VertexArray::~VertexArray()
		{
		}

		VertexArray& VertexArray::operator=(VertexArray &&rhs)
		{
			MyBase::operator=(std::move(rhs));
			return *this;
		}

		void VertexArray::create()
		{
			if (m_id == 0)
			{
				glCreateVertexArrays(1, &m_id);
			}
		}

		void VertexArray::destroy()
		{
			if (m_id != 0)
			{
				glBindVertexArray(0);
				glDeleteVertexArrays(1, &m_id);
			}
		}

		void VertexArray::bind()
		{
			glBindVertexArray(getId());
		}

		void VertexArray::bindZero()
		{
			glBindVertexArray(0);
		}

		void VertexArray::enableArrayAttrib(U32 index)
		{
			glEnableVertexArrayAttrib(getId(), index);
		}

		void VertexArray::arrayAttribBinding(U32 attribIndex, U32 bindingIndex)
		{
			glVertexArrayAttribBinding(getId(), attribIndex, bindingIndex);
		}

		void VertexArray::arrayAttribFormatI(U32 attribIndex, U32 size, DataType type, U32 relativeOffset)
		{
			U32 dataType = 0;
			switch (type)
			{
			case vx::gl::DataType::Unsigned_Int:
				dataType = GL_UNSIGNED_INT;
				break;
			case vx::gl::DataType::Unsigned_Byte:
				dataType = GL_UNSIGNED_BYTE;
				break;
			case vx::gl::DataType::Byte:
				dataType = GL_BYTE;
				break;
			case vx::gl::DataType::Unsigned_Short:
				dataType = GL_UNSIGNED_SHORT;
				break;
			case vx::gl::DataType::Short:
				dataType = GL_SHORT;
				break;
			case vx::gl::DataType::Int:
				dataType = GL_INT;
				break;
			default:
				// only valid for integer types
				VX_ASSERT(false);
				break;
			}

			glVertexArrayAttribIFormat(getId(), attribIndex, size, dataType, relativeOffset);
		}

		void VertexArray::arrayAttribFormatF(U32 attribIndex, U32 size, U8 normalized, U32 relativeOffset)
		{
			glVertexArrayAttribFormat(getId(), attribIndex, size, GL_FLOAT, normalized, relativeOffset);
		}

		void VertexArray::arrayBindingDivisor(U32 bindingIndex, U32 divisor)
		{
			glVertexArrayBindingDivisor(getId(), bindingIndex, divisor);
		}

		void VertexArray::bindIndexBuffer(U32 ibo)
		{
			glVertexArrayElementBuffer(getId(), ibo);
		}

		void VertexArray::bindVertexBuffer(U32 vbo, U32 bindingIndex, size_t offset, size_t stride)
		{
			glVertexArrayVertexBuffer(getId(), bindingIndex, vbo, offset, stride);
		}
	}
}