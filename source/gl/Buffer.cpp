#include <vxLib/gl/Buffer.h>
#include <vxLib/gl/gl.h>

namespace vx
{
	namespace gl
	{
		namespace detail
		{
			const U32 g_bufferTypes[14] = 
			{
				GL_ARRAY_BUFFER,
				GL_ATOMIC_COUNTER_BUFFER,
				GL_COPY_READ_BUFFER,
				GL_COPY_WRITE_BUFFER,
				GL_DRAW_INDIRECT_BUFFER,
				GL_DISPATCH_INDIRECT_BUFFER,
				GL_ELEMENT_ARRAY_BUFFER,
				GL_PIXEL_PACK_BUFFER,
				GL_PIXEL_UNPACK_BUFFER,
				GL_QUERY_BUFFER,
				GL_SHADER_STORAGE_BUFFER,
				GL_TEXTURE_BUFFER,
				GL_TRANSFORM_FEEDBACK_BUFFER,
				GL_UNIFORM_BUFFER
			};

			inline U32 getBufferType(BufferType type)
			{
				return g_bufferTypes[(U32)type];
			}

			bool BufferInterface::create(const BufferDescription &desc, U32 &id)
			{
				if (id == 0)
				{
					glCreateBuffers(1, &id);

					if (id == 0)
						return false;
					if (desc.immutable == 0)
					{
						glNamedBufferData(id, desc.size, desc.pData, (U32)desc.usage);
					}
					else
					{
						glNamedBufferStorage(id, desc.size, desc.pData, (U32)desc.flags);
					}

					return true;
				}

				return false;
			}

			void BufferInterface::destroy(U32 &id)
			{
				if (id != 0)
				{
					glDeleteBuffers(1, &id);
					id = 0;
				}
			}

			void BufferInterface::bind(U32 id, BufferType type)
			{
				glBindBuffer(getBufferType(type), id);
			}

			U32 BufferInterface::getTarget(BufferType type)
			{
				return getBufferType(type);
			}

			void* BufferInterface::map(U32 id, Map access)
			{
				return glMapNamedBuffer(id, (U32)access);
			}

			void* BufferInterface::mapRange(U32 id, U32 offsetBytes, U32 sizeBytes, MapRange::Access access)
			{
				return glMapNamedBufferRange(id, offsetBytes, sizeBytes, (U32)access);
			}

			void BufferInterface::unmap(U32 id)
			{
				glUnmapNamedBuffer(id);
			}
		}

		Buffer::Buffer()
			:Base(),
			m_target(0)
		{
		}

		Buffer::Buffer(Buffer &&rhs)
			: Base(std::move(rhs)),
			m_target(rhs.m_target)
		{

		}

		Buffer& Buffer::operator = (Buffer &&rhs)
		{
			if (this != &rhs)
			{
				Base::operator=(std::move(rhs));
				std::swap(m_target, rhs.m_target);
			}
			return *this;
		}

		bool Buffer::create(const BufferDescription &desc)
		{
			auto target = ::vx::gl::detail::getBufferType(desc.bufferType);
			m_target = target | ((U8)desc.bufferType >> 24);
			return detail::BufferInterface::create(desc, m_id);
		}

		void Buffer::destroy()
		{
			return detail::BufferInterface::destroy(m_id);
		}

		void Buffer::bind() const
		{
			glBindBuffer((m_target & 0x00FFFFFF), m_id);
		}

		void* Buffer::map(Map access)
		{
			auto ptr = detail::BufferInterface::map(m_id, access);
			return ptr;
		}

		void* Buffer::mapRange(U32 offsetBytes, U32 sizeBytes, MapRange::Access access)
		{
			auto ptr = detail::BufferInterface::mapRange(m_id, offsetBytes, sizeBytes, access);
			return ptr;
		}

		void Buffer::unmap() const
		{
			detail::BufferInterface::unmap(m_id);
		}

		void Buffer::subData(I64 offset, I64 size,const void* data)
		{
			glNamedBufferSubData(m_id, offset, size, data);
		}

		U32 Buffer::getTarget() const
		{
			return (m_target & 0x00FFFFFF);
		}

		BufferType Buffer::getType() const
		{
			return BufferType((m_target >> 24));
		}
	}
}
