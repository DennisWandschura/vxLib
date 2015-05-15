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

#include <vxLib/gl/Base.h>

namespace vx
{
	namespace gl
	{
		enum class Attachment : u8
		{
			Color0,
			Color1,
			Color2,
			Color3,
			Color4,
			Color5,
			Color6,
			Color7,
			Color8,
			Color9,
			Color10,
			Color11,
			Color12,
			Color13,
			Color14,
			Color15,
			Depth,
			Stencil,
			Depth_Stencil
		};

		class Framebuffer : public Base < Framebuffer >
		{
		public:
			Framebuffer();
			Framebuffer(const Framebuffer&) = delete;
			Framebuffer(Framebuffer &&rhs) noexcept;

			Framebuffer& operator=(const Framebuffer&) = delete;
			Framebuffer& operator=(Framebuffer &&rhs) noexcept;

			void create();
			void destroy();

			u32 checkStatus();

			void bind();
			void bindZero();

			void attachTexture(Attachment attachment, u32 textureId, u32 level);
			void attachTextureLayer(Attachment attachment, u32 textureId, u32 level, u32 layer);

			template<class Texture>
			void attachTexture(Attachment attachment, const Texture &texture, u32 level)
			{
				attachTexture(attachment, texture.getId(), level);
			}

			template<class Texture>
			void attachTextureLayer(Attachment attachment, const Texture &texture, u32 level, u32 layer)
			{
				attachTextureLayer(attachment, texture.getId(), level, layer);
			}
		};
	}
}