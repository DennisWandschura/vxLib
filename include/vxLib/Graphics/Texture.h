#pragma once

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

#include <vxlib/Graphics/Surface.h>
#include <vector>

namespace vx
{
	namespace Graphics
	{
		enum class TextureFormat : u8
		{
			Unkown,
			RED,
			BG,
			BGR,
			BGRA,
			RG,
			RGB,
			RGBA,
			SRGBA,
			DXT1,
			DXT3,
			DXT5,
			BC7_UNORM_SRGB,
			BC7_UNORM,
			BC6H_UF16,
			BC6H_SF16
		};

		enum class TextureType : u8 { Flat, Cubemap, Volume };

		namespace detail
		{
			extern u32 getRowPitchBlock(u32 width, u32 blockSize);
			extern u32 getRowPitchNormal(u32 width, u32 components);

			extern u32 getSizeNormal(const vx::uint2 &dim, u32 components);
			extern u32 getSizeBlock(const vx::uint2 &dim, u32 blockSize);
		}

		extern u32 getRowPitch(TextureFormat format, u32 width);
		extern u32 getTextureSize(TextureFormat format, const vx::uint2 &dim);

		extern u32 textureFormatToDxgiFormat(TextureFormat format);
		extern TextureFormat dxgiFormatToTextureFormat(u32 format);

		class Face : public Surface
		{
			Surface* m_mipmaps;
			u32 m_mipmapCount;
			u32 m_padding;
			size_t m_allocatedSize;

		public:
			Face();
			Face(const Face&) = delete;
			Face(Face &&rhs);
			~Face();

			Face& operator=(const Face&) = delete;
			Face& operator=(Face &&rhs);

			void create(const vx::uint3 &dimension, u32 size, const vx::AllocatedBlock &pixels);
			void setMipmaps(const vx::AllocatedBlock &mipmaps, u32 count);

			template<typename Allocator>
			void release(Allocator* allocator)
			{
				auto pixelBlock = Surface::release();
				vx::AllocatedBlock mipmapBlock = { (u8*)m_mipmaps, m_allocatedSize };

				for (u32 i = 0; i < m_mipmapCount; ++i)
				{
					auto block = m_mipmaps[i].release();
					m_mipmaps[i].~Surface();
					allocator->deallocate(block);
				}
				m_mipmapCount = 0;

				allocator->deallocate(pixelBlock);
				allocator->deallocate(mipmapBlock);

				m_mipmaps = nullptr;
				m_allocatedSize = 0;
			}

			u32 getMipmapCount() const { return m_mipmapCount; }
			Surface* getMipmap(u32 i) { return &m_mipmaps[i]; }
			const Surface* getMipmap(u32 i) const { return &m_mipmaps[i]; }
		};

		class Texture
		{
			Face* m_faces;
			u32 m_faceCount;
			TextureFormat m_format;
			TextureType m_type;
			u8 m_components;
			size_t m_allocatedSize;

		public:
			Texture();
			Texture(const Texture&) = delete;
			Texture(Texture &&rhs);
			~Texture();

			Texture& operator=(const Texture&) = delete;
			Texture& operator=(Texture &&rhs);

			void create(const vx::AllocatedBlock &faces, TextureFormat format, TextureType type, u8 components);

			template<typename Allocator>
			void release(Allocator* allocator)
			{
				for (u32 i = 0; i < m_faceCount; ++i)
				{
					m_faces[i].release(allocator);
				}
				vx::AllocatedBlock faceBlock = { (u8*)m_faces, m_allocatedSize };
				allocator->deallocate(faceBlock);

				m_faces = nullptr;
				m_faceCount = 0;
				m_components = 0;
				m_allocatedSize = 0;
			}

			Face& getFace(u32 i) { return m_faces[i]; }
			const Face& getFace(u32 i) const { return m_faces[i]; }

			TextureFormat getFormat() const { return m_format; }

			u32 getFaceSize(u32 i) const;
			u32 getFaceRowPitch(u32 i) const;

			u32 getFaceCount() const { return m_faceCount; }
			u8 getComponents() const { return m_components; }
		};
	}
}