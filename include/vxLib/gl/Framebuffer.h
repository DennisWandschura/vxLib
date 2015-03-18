#ifndef __VX_GL_FRAMEBUFFER_H
#define __VX_GL_FRAMEBUFFER_H
#pragma once

#include <vxLib/gl/Base.h>

namespace vx
{
	namespace gl
	{
		enum class Attachment : U8
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

			U32 checkStatus();

			void bind();
			void bindZero();

			void attachTexture(Attachment attachment, U32 textureId, U32 level);
			void attachTextureLayer(Attachment attachment, U32 textureId, U32 level, U32 layer);

			template<class Texture>
			void attachTexture(Attachment attachment, const Texture &texture, U32 level)
			{
				attachTexture(attachment, texture.getId(), level);
			}

			template<class Texture>
			void attachTextureLayer(Attachment attachment, const Texture &texture, U32 level, U32 layer)
			{
				attachTextureLayer(attachment, texture.getId(), level, layer);
			}
		};
	}
}
#endif