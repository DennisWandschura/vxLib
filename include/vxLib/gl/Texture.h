#ifndef __VX_GL_TEXTURE_H
#define __VX_GL_TEXTURE_H
#pragma once

#include <vxLib\gl\Base.h>
#include <vxLib\math\Vector.h>
#include <vxLib\Container\bitset.h>

namespace vx
{
	namespace gl
	{
		enum class TextureType : U8
		{
			Texture_1D,
			Texture_2D,
			Texture_2D_MS,
			Texture_3D,
			Texture_Cubemap,
			Texture_Cubemap_Array,
			Texture_1D_Array,
			Texture_2D_Array,
			Texture_2D_MS_Array
		};

		enum class TextureFormat : U8
		{
			R8,
			R8S,
			R16,
			R16S,
			RG8,
			RG8S,
			RG16,
			RG16S,
			RGB4,
			RGB5,
			RGB8,
			RGB8S,
			RGB10,
			RGB12,
			RGB16S,
			RGBA2,
			RGBA4,
			RGB5A1,
			RGBA8,
			RGBA8S,
			RGB10A2,
			RGB10A2UI,
			RGBA12,
			RGBA16,
			SRGB8,
			SRGBA8,
			RGB_DXT1,
			RGBA_DXT1,
			RGBA_DXT3,
			RGBA_DXT5,
			SRGB_DXT1,
			SRGBA_DXT1,
			SRGBA_DXT3,
			SRGBA_DXT5,
			R16F,
			RG16F,
			RGB16F,
			RGBA16F,
			R32F,
			RG32F,
			RGB32F,
			RGBA32F,
			R8I,
			R8UI,
			R16I,
			R16UI,
			R32I,
			R32UI,
			RG8I,
			RG8UI,
			RG16I,
			RG16UI,
			RG32I,
			RG32UI,
			RGB8I,
			RGB8UI,
			RGB16I,
			RGB16UI,
			RGB32I,
			RGB32UI,
			RGBA8I,
			RGBA8UI,
			RGBA16I,
			RGBA16UI,
			RGBA32I,
			RGBA32UI,
			DEPTH16,
			DEPTH24,
			DEPTH32,
			DEPTH32F
		};

		struct TextureDescription
		{
			vx::ushort3 size;
			union
			{
				U16 miplevels;
				U16 samples; // only used by multisample
			};

			TextureType type;
			TextureFormat format;
			U8 fixedsamplelocations; // only used by multisample
			U8 sparse; // if sparse textures are supported

			TextureDescription() :size(), miplevels(0), type((gl::TextureType)0), format((gl::TextureFormat)0), fixedsamplelocations(0), sparse(0){}

			static TextureDescription create(TextureType type, TextureFormat format, const vx::ushort3 &size, U16 miplevels, U8 sparse);
			static TextureDescription createMultisampled(TextureType type, TextureFormat format, const vx::ushort3 &size, U16 samples, U8 fixedsamplelocations, U8 sparse);
		};

		struct TextureCommitDescription
		{
			U32 miplevel;
			vx::uint3 offset;
			vx::uint3 size;
			U8 commit;
		};

		struct TextureSubImageDescription
		{
			U32 miplevel;
			vx::uint3 offset;
			vx::uint3 size;
			DataType dataType;
			const void *p;
		};

		class Texture : public Base < Texture >
		{
			U32 m_target;
			U32 m_format;
			U32 m_internalFormat;
			vx::ushort3 m_size;
			vx::bitset<4> m_formatData; // 1. bit sparse, 2. bit 1d, 3. bit 2d, 4. bit 3d

			void allocate1D(const TextureDescription &desc);
			void allocate2D(const TextureDescription &desc);
			void allocate2D_MS(const TextureDescription &desc);
			void allocate3D(const TextureDescription &desc);

		public:
			Texture();

			Texture(const Texture&) = delete;
			Texture(Texture &&rhs) noexcept;

			~Texture();

			Texture& operator=(const Texture&) = delete;
			Texture& operator=(Texture &&rhs) noexcept;

			void create(const TextureDescription &desc);
			void destroy();

			void bind() const;

			// only used by sparse textures
			void commit(const TextureCommitDescription &desc);

			void subImage(const TextureSubImageDescription &desc);

			void setWrapMode1D(TextureWrapMode wrap_s);
			void setWrapMode2D(TextureWrapMode wrap_s, TextureWrapMode wrap_t);
			void setWrapMode3D(TextureWrapMode wrap_s, TextureWrapMode wrap_t, TextureWrapMode wrap_r);
			void setFilter(TextureFilter min, TextureFilter mag);
			void makeTextureResident();
			void makeTextureNonResident();
			void generateMipmaps();
			void clearImage(U32 level, U32 format, U32 type, const void* data);

			U32 getId() const;
			U64 getTextureHandle() const;
			U64 getImageHandle(U32 level, U8 layered, U32 layer) const;
			const vx::ushort3& getSize() const;
			U32 getTarget() const;

			bool isSparseTexture() const;
			bool is1D() const;
			bool is2D() const;
			bool is3D() const;
		};
	}
}
#endif