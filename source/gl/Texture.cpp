#include <vxLib\gl\Texture.h>
#include <vxLib/gl/gl.h>
#include <cstdio>

namespace vx
{
	namespace gl
	{
		static TextureDescription create(TextureType type, TextureFormat format, const vx::ushort3 &size, U16 miplevels, U8 sparse)
		{
			TextureDescription desc;
			desc.type = type;
			desc.format = format;
			desc.size = size;
			desc.miplevels = miplevels;
			desc.sparse = sparse;

			return desc;
		}

		static TextureDescription createMultisampled(TextureType type, TextureFormat format, const vx::ushort3 &size, U16 samples, U8 fixedsamplelocations, U8 sparse)
		{
			TextureDescription desc;
			desc.type = type;
			desc.format = format;
			desc.size = size;
			desc.samples = samples;
			desc.fixedsamplelocations = fixedsamplelocations;
			desc.sparse = sparse;

			return desc;
		}

		void getTextureFormat(TextureFormat textureFormat, U32 &format, U32 &internalFormat)
		{
			format = 0;
			internalFormat = 0;

			switch (textureFormat)
			{
			case gl::R8:
			{
				format = GL_RED;
				internalFormat = GL_R8;
			}break;
			case gl::R8S:
			{
				format = GL_RED;
				internalFormat = GL_R8_SNORM;
			}break;
			case gl::R16:
			{
				format = GL_RED;
				internalFormat = GL_R16;
			}break;
			case gl::R16S:
			{
				internalFormat = GL_R16_SNORM;
				format = GL_RED;
			}break;
			case gl::RG8:
			{
				internalFormat = GL_RGB8;
				format = GL_RG;
			}break;
			case gl::RG8S:
			{
				internalFormat = GL_RG8_SNORM;
				format = GL_RG;
			}break;
			case gl::RG16:
			{
				internalFormat = GL_RGB16;
				format = GL_RG;
			}break;
			case gl::RG16S:
			{
				internalFormat = GL_RGB16_SNORM;
				format = GL_RG;
			}break;
			case gl::RGB4:
			{
				internalFormat = GL_RGB4;
				format = GL_RGB;
			}break;
			case gl::RGB5:
			{
				internalFormat = GL_RGB5;
				format = GL_RGB;
			}break;
			case gl::RGB8:
			{
				internalFormat = GL_RGB8;
				format = GL_RGB;
			}break;
			case gl::RGB8S:
			{
				internalFormat = GL_RGB8_SNORM;
				format = GL_RGB;
			}break;
			case gl::RGB10:
			{
				internalFormat = GL_RGB10;
				format = GL_RGB;
			}break;
			case gl::RGB12:
			{
				internalFormat = GL_RGB12;
				format = GL_RGB;
			}break;
			case gl::RGB16S:
			{
				internalFormat = GL_RGB16_SNORM;
				format = GL_RGB;
			}break;
			case gl::RGBA2:
			{
				internalFormat = GL_RGBA2;
				format = GL_RGBA;
			}break;
			case gl::RGBA4:
			{
				internalFormat = GL_RGBA4;
				format = GL_RGBA;
			}break;
			case gl::RGB5A1:
			{
				internalFormat = GL_RGB5_A1;
				format = GL_RGBA;
			}break;
			case gl::RGBA8:
			{
				internalFormat = GL_RGBA8;
				format = GL_RGBA;
			}break;
			case gl::RGBA8S:
			{
				internalFormat = GL_RGBA8_SNORM;
				format = GL_RGBA;
			}break;
			case gl::RGB10A2:
			{
				format = GL_RGB;
				internalFormat = GL_RGB10_A2;
			}break;
			case gl::RGB10A2UI:
			{
				format = GL_RGBA;
				internalFormat = GL_RGB10_A2UI;
			}break;
			case gl::RGBA12:
			{
				internalFormat = GL_RGBA12;
				format = GL_RGBA;
			}break;
			case gl::RGBA16:
			{
				internalFormat = GL_RGBA16;
				format = GL_RGBA;
			}break;
			case gl::SRGB8:
			{
				format = GL_RGB;
				internalFormat = GL_SRGB8;
			}break;
			case gl::SRGBA8:
			{
				format = GL_RGBA;
				internalFormat = GL_SRGB8_ALPHA8;
			}break;
			case gl::RGB_DXT1:
			{
				format = GL_RGB;
				internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			}break;
			case gl::RGBA_DXT1:
			{
				format = GL_RGBA;
				internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			}break;
			case gl::RGBA_DXT3:
			{
				format = GL_RGBA;
				internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			}break;
			case gl::RGBA_DXT5:
			{
				format = GL_RGBA;
				internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			}break;
			case gl::SRGB_DXT1:
			{
				format = GL_RGB;
				internalFormat = GL_COMPRESSED_SRGB_S3TC_DXT1_EXT;
			}break;
			case gl::SRGBA_DXT1:
			{
				format = GL_RGBA;
				internalFormat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
			}break;
			case gl::SRGBA_DXT3:
			{
				format = GL_RGBA;
				internalFormat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
			}break;
			case gl::SRGBA_DXT5:
			{
				format = GL_RGBA;
				internalFormat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
			}break;
			case gl::R16F:
			{
				format = GL_RED;
				internalFormat = GL_R16F;
			}break;
			case gl::RG16F:
			{
				format = GL_RG;
				internalFormat = GL_RG16F;
			}break;
			case gl::RGB16F:
			{
				format = GL_RGB;
				internalFormat = GL_RGB16F;
			}break;
			case gl::RGBA16F:
			{
				format = GL_RGBA;
				internalFormat = GL_RGBA16F;
			}break;
			case gl::R32F:
			{
				format = GL_RED;
				internalFormat = GL_R32F;
			}break;
			case gl::RG32F:
			{
				format = GL_RG;
				internalFormat = GL_RG32F;
			}break;
			case gl::RGB32F:
			{
				format = GL_RGB;
				internalFormat = GL_RGB32F;
			}break;
			case gl::RGBA32F:
			{
				format = GL_RGBA;
				internalFormat = GL_RGBA32F;
			}break;
			case gl::R8I:
			{
				format = GL_RED;
				internalFormat = GL_R8I;
			}break;
			case gl::R8UI:
			{
				format = GL_RED;
				internalFormat = GL_R8UI;
			}break;
			case gl::R16I:
			{
				format = GL_RED;
				internalFormat = GL_R16I;
			}break;
			case gl::R16UI:
			{
				format = GL_RED;
				internalFormat = GL_R16UI;
			}break;
			case gl::R32I:
			{
				format = GL_RED;
				internalFormat = GL_R32I;
			}break;
			case gl::R32UI:
			{
				format = GL_RED;
				internalFormat = GL_R32UI;
			}break;
			case gl::RG8I:
			{
				internalFormat = GL_RG8I;
				format = GL_RG;
			}break;
			case gl::RG8UI:
			{
				internalFormat = GL_RG8UI;
				format = GL_RG;
			}break;
			case gl::RG16I:
			{
				internalFormat = GL_RG16I;
				format = GL_RG;
			}break;
			case gl::RG16UI:
			{
				internalFormat = GL_RG16UI;
				format = GL_RG;
			}break;
			case gl::RG32I:
			{
				internalFormat = GL_RG32I;
				format = GL_RG;
			}break;
			case gl::RG32UI:
			{
				internalFormat = GL_RG32UI;
				format = GL_RG;
			}break;
			case gl::RGB8I:
			{
				internalFormat = GL_RGB8I;
				format = GL_RGB;
			}break;
			case gl::RGB8UI:
			{
				internalFormat = GL_RGB8UI;
				format = GL_RGB;
			}break;
			case gl::RGB16I:
			{
				internalFormat = GL_RGB16I;
				format = GL_RGB;
			}break;
			case gl::RGB16UI:
			{
				internalFormat = GL_RGB16UI;
				format = GL_RGB;
			}break;
			case gl::RGB32I:
			{
				internalFormat = GL_RGB32I;
				format = GL_RGB;
			}break;
			case gl::RGB32UI:
			{
				internalFormat = GL_RGB32UI;
				format = GL_RGB;
			}break;
			case gl::RGBA8I:
			{
				internalFormat = GL_RGBA8I;
				format = GL_RGBA;
			}break;
			case gl::RGBA8UI:
			{
				internalFormat = GL_RGBA8UI;
				format = GL_RGBA;
			}break;
			case gl::RGBA16I:
			{
				internalFormat = GL_RGBA16I;
				format = GL_RGBA;
			}break;
			case gl::RGBA16UI:
			{
				internalFormat = GL_RGBA16UI;
				format = GL_RGBA;
			}break;
			case gl::RGBA32I:
			{
				internalFormat = GL_RGBA32I;
				format = GL_RGBA;
			}break;
			case gl::RGBA32UI:
			{
				internalFormat = GL_RGBA32UI;
				format = GL_RGBA;
			}break;
			case DEPTH16:
			{
				internalFormat = GL_DEPTH_COMPONENT16;
				format = GL_DEPTH_COMPONENT;
			}break;
			case DEPTH24:
			{
				internalFormat = GL_DEPTH_COMPONENT24;
				format = GL_DEPTH_COMPONENT;
			}break;
			case DEPTH32:
			{
				internalFormat = GL_DEPTH_COMPONENT32;
				format = GL_DEPTH_COMPONENT;
			}break;
			case DEPTH32F:
			{
				internalFormat = GL_DEPTH_COMPONENT32F;
				format = GL_DEPTH_COMPONENT;
			}break;
			default:
				VX_ASSERT(false);
				break;
			}
		}

		namespace detail
		{
			U32 getTarget(TextureType type)
			{
				auto target = 0u;
				switch (type)
				{
				case gl::Texture_1D:
					target = GL_TEXTURE_1D;
					break;
				case gl::Texture_2D:
					target = GL_TEXTURE_2D;
					break;
				case gl::Texture_2D_MS:
					target = GL_TEXTURE_2D_MULTISAMPLE;
					break;
				case gl::Texture_3D:
					target = GL_TEXTURE_3D;
					break;
				case gl::Texture_Cubemap:
					target = GL_TEXTURE_CUBE_MAP;
					break;
				case gl::Texture_Cubemap_Array:
					target = GL_TEXTURE_CUBE_MAP_ARRAY;
					break;
				case gl::Texture_1D_Array:
					target = GL_TEXTURE_1D_ARRAY;
					break;
				case gl::Texture_2D_Array:
					target = GL_TEXTURE_2D_ARRAY;
					break;
				case gl::Texture_2D_MS_Array:
					target = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
					break;
				default:
					VX_ASSERT(false);
					break;
				}

				return target;
			}

			void subImage2D
				(
				U32 id,
				U32 level,
				U32 xoffset,
				U32 yoffset,
				U32 width,
				U32 height,
				U32 format,
				DataType dataType,
				const void *p
				)
			{
				auto dtaType = ::vx::gl::detail::getDataType(dataType);

				glTextureSubImage2D(id, level, xoffset, yoffset, width, height, format, dtaType, p);
			}

			void subImage2D(
				U32 id,
				GLenum target, 
				U32 level,
				U32 xoffset,
				U32 yoffset,
				U32 width,
				U32 height,
				U32 format,
				DataType dataType,
				const void *p
				)
			{
				auto dtaType = ::vx::gl::detail::getDataType(dataType);

				glBindTexture(target, id);
				glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, dtaType, p);
				glBindTexture(target, 0);
			}

			void subImage3D
				(
				U32 id,
				U32 level,
				U32 xoffset,
				U32 yoffset,
				U32 zoffset,
				U32 width,
				U32 height,
				U32 depth,
				U32 format,
				DataType dataType,
				const void *p
				)
			{
				auto dtaType = getDataType(dataType);

				glTextureSubImage3D(id, level, xoffset, yoffset, zoffset, width, height, depth, format, dtaType, p);
			}
		}

		/*

		Texture

		*/

		Texture::Texture()
			:Base(),
			m_target(0),
			m_format(0),
			m_internalFormat(0),
			m_size(),
			m_formatData()
		{
		}

		Texture::Texture(Texture &&rhs) noexcept
			: Base(std::move(rhs)),
			m_target(rhs.m_target),
			m_format(rhs.m_format),
			m_internalFormat(rhs.m_internalFormat),
			m_size(rhs.m_size),
			m_formatData(std::move(rhs.m_formatData))
		{
		}

		Texture::~Texture()
		{
		}

		Texture& Texture::operator = (Texture &&rhs) noexcept
		{
			if (this != &rhs)
			{
				Base::operator=(std::move(rhs));

				m_target = rhs.m_target;
				m_format = rhs.m_format;
				m_internalFormat = rhs.m_internalFormat;
				m_size = rhs.m_size;
				m_formatData = rhs.m_formatData;
			}

			return *this;
		}

		void Texture::allocate1D(const TextureDescription &desc)
		{
			assert(desc.miplevels >= 1);
			assert(desc.size.x != 0);
			assert(m_internalFormat != 0);

			glTextureStorage1D(m_id, desc.miplevels, m_internalFormat, desc.size.x);
		}

		void Texture::allocate2D(const TextureDescription &desc)
		{
			assert(desc.miplevels >= 1);
			assert(desc.size.x != 0);
			assert(desc.size.y != 0);
			assert(m_internalFormat != 0);

			glTextureStorage2D(m_id, desc.miplevels, m_internalFormat, desc.size.x, desc.size.y);
		}

		void Texture::allocate2D_MS(const TextureDescription &desc)
		{
			assert(desc.samples != 0);
			assert(desc.size.x != 0);
			assert(desc.size.y != 0);
			assert(m_internalFormat != 0);

			glTextureStorage2DMultisample(m_id, desc.samples, m_internalFormat, desc.size.x, desc.size.y, desc.fixedsamplelocations);
		}

		void Texture::allocate3D(const TextureDescription &desc)
		{
			assert(desc.miplevels >= 1);
			assert(desc.size.x != 0);
			assert(desc.size.y != 0);
			assert(desc.size.z != 0);
			assert(m_internalFormat != 0);

			glTextureStorage3D(m_id, desc.miplevels, m_internalFormat, desc.size.x, desc.size.y, desc.size.z);
		}

		void Texture::create(const TextureDescription &desc)
		{
			if (m_id == 0)
			{
				m_target = ::vx::gl::detail::getTarget(desc.type);
				m_size = desc.size;

				getTextureFormat(desc.format, m_format, m_internalFormat);

				glCreateTextures(m_target, 1, &m_id);

				if (desc.sparse == 1)
				{
					m_formatData.set<0>();

					glTextureParameteri(m_id, GL_TEXTURE_SPARSE_ARB, GL_TRUE);
				}

				switch (desc.type)
				{
				case gl::Texture_2D:
					m_formatData.set<2>();
					allocate2D(desc);
					break;
				case gl::Texture_2D_Array:
					m_formatData.set<2>();
					allocate3D(desc);
					break;
				case gl::Texture_3D:
					m_formatData.set<3>();
					allocate3D(desc);
					break;
				case gl::Texture_1D:
					m_formatData.set<1>();
					allocate1D(desc);
					break;
				case gl::Texture_2D_MS:
					m_formatData.set<2>();
					allocate2D_MS(desc);
					break;
				case gl::Texture_Cubemap:
					allocate2D(desc);
					m_formatData.set<2>();
					break;
				case gl::Texture_Cubemap_Array:
					allocate3D(desc);
					m_formatData.set<3>();
					break;
				default:
					VX_ASSERT(false);
					break;
				}
			}
		}

		void Texture::destroy()
		{
			if (m_id != 0)
			{
				glDeleteTextures(1, &m_id);
				m_id = 0;
			}
		}

		void Texture::bind() const
		{
			glBindTexture(m_target, m_id);
		}

		void Texture::subImage(const TextureSubImageDescription &desc)
		{
			switch (m_target)
			{
			case GL_TEXTURE_2D_ARRAY:
				vx::gl::detail::subImage3D(m_id, desc.miplevel, desc.offset.x, desc.offset.y, desc.offset.z, desc.size.x, desc.size.y, desc.size.z, m_format, desc.dataType, desc.p);
				break;
			case GL_TEXTURE_2D:
				vx::gl::detail::subImage2D(m_id, desc.miplevel, desc.offset.x, desc.offset.y, desc.size.x, desc.size.y, m_format, desc.dataType, desc.p);
				break;
			case GL_TEXTURE_3D:
				vx::gl::detail::subImage3D(m_id, desc.miplevel, desc.offset.x, desc.offset.y, desc.offset.z, desc.size.x, desc.size.y, desc.size.z, m_format, desc.dataType, desc.p);
				break;
			case GL_TEXTURE_1D_ARRAY:
				vx::gl::detail::subImage2D(m_id, desc.miplevel, desc.offset.x, desc.offset.y, desc.size.x, desc.size.y, m_format, desc.dataType, desc.p);
				break;
			case GL_TEXTURE_CUBE_MAP:
			{
				::vx::gl::detail::subImage2D(m_id, GL_TEXTURE_CUBE_MAP_POSITIVE_X + desc.offset.z, desc.miplevel, desc.offset.x, desc.offset.y, desc.size.x, desc.size.y, m_format, desc.dataType, desc.p);
			}break;
			default:
				VX_ASSERT(false);
				break;
			}
		}

		void Texture::commit(const TextureCommitDescription &desc)
		{
			if (!isSparseTexture())
				return;
			glTexturePageCommitmentEXT(m_id, desc.miplevel, desc.offset.x, desc.offset.y, desc.offset.z, desc.size.x, desc.size.y, desc.size.z, desc.commit);
		}

		void Texture::setWrapMode1D(TextureWrapMode wrap_s)
		{
			auto s = detail::getTextureWrapMode(wrap_s);
			glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, s);
		}

		void Texture::setWrapMode2D(TextureWrapMode wrap_s, TextureWrapMode wrap_t)
		{
			setWrapMode1D(wrap_s);

			auto t = detail::getTextureWrapMode(wrap_t);
			glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, t);
		}

		void Texture::setWrapMode3D(TextureWrapMode wrap_s, TextureWrapMode wrap_t, TextureWrapMode wrap_r)
		{
			setWrapMode2D(wrap_s, wrap_t);

			auto r = detail::getTextureWrapMode(wrap_r);
			glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, r);
		}

		void Texture::setFilter(TextureFilter min, TextureFilter mag)
		{
			auto minFilter = detail::getTextureFilter(min);
			auto magFilter = detail::getTextureFilter(mag);

			glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, minFilter);
			glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, magFilter);
		}

		void Texture::makeTextureResident()
		{
			auto handle = getTextureHandle();
			glMakeTextureHandleResidentARB(handle);
		}

		void Texture::makeTextureNonResident()
		{
			auto handle = getTextureHandle();
			glMakeTextureHandleNonResidentARB(handle);
		}

		void Texture::generateMipmaps()
		{
			glGenerateTextureMipmap(m_id);
		}

		void Texture::clearImage(U32 level, U32 format, U32 type, const void* data)
		{
			glClearTexImage(m_id, level, format, type, data);
		}

		U32 Texture::getId() const
		{
			return m_id;
		}

		U64 Texture::getTextureHandle() const
		{
			return glGetTextureHandleARB(m_id);
		}

		U64 Texture::getImageHandle(U32 level, U8 layered, U32 layer) const
		{
			return glGetImageHandleARB(m_id, level, layered, layer, m_internalFormat);
		}

		const vx::ushort3& Texture::getSize() const
		{
			return m_size;
		}

		U32 Texture::getTarget() const
		{
			return m_target;
		}

		bool Texture::isSparseTexture() const
		{
			return m_formatData.get<0>();
		}

		bool Texture::is1D() const
		{
			return m_formatData.get<1>();
		}

		bool Texture::is2D() const
		{
			return m_formatData.get<2>();
		}

		bool Texture::is3D() const
		{
			return m_formatData.get<3>();
		}
	}
}