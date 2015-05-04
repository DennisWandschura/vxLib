/*
The MIT License(MIT)

Copyright(c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <vxLib/gl/Framebuffer.h>
#include <vxLib/gl/gl.h>

namespace vx
{
	namespace gl
	{
		namespace detail
		{
			U32 g_attachments[19] =
			{
				GL_COLOR_ATTACHMENT0,
				GL_COLOR_ATTACHMENT1,
				GL_COLOR_ATTACHMENT2,
				GL_COLOR_ATTACHMENT3,
				GL_COLOR_ATTACHMENT4,
				GL_COLOR_ATTACHMENT5,
				GL_COLOR_ATTACHMENT6,
				GL_COLOR_ATTACHMENT7,
				GL_COLOR_ATTACHMENT8,
				GL_COLOR_ATTACHMENT9,
				GL_COLOR_ATTACHMENT10,
				GL_COLOR_ATTACHMENT11,
				GL_COLOR_ATTACHMENT12,
				GL_COLOR_ATTACHMENT13,
				GL_COLOR_ATTACHMENT14,
				GL_COLOR_ATTACHMENT15,
				GL_DEPTH_ATTACHMENT,
				GL_STENCIL_ATTACHMENT,
				GL_DEPTH_STENCIL_ATTACHMENT
			};
		}

		inline U32 getAttachment(Attachment at)
		{
			return detail::g_attachments[(U8)at];
		}

		Framebuffer::Framebuffer()
			:Base()
		{
		}

		Framebuffer::Framebuffer(Framebuffer &&rhs) noexcept
			: Base(std::move(rhs))
		{
		}

		Framebuffer& Framebuffer::operator = (Framebuffer &&rhs) noexcept
		{
			Base::operator=(std::move(rhs));
			return *this;
		}

			void Framebuffer::create()
		{
			if (m_id == 0)
			{
				glCreateFramebuffers(1, &m_id);
			}
		}

		void  Framebuffer::destroy()
		{
			if (m_id != 0)
			{
				glDeleteFramebuffers(1, &m_id);
				m_id = 0;
			}
		}

		U32 Framebuffer::checkStatus()
		{
			return glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER);
		}

		void Framebuffer::bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		}

		void Framebuffer::bindZero()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void Framebuffer::attachTexture(Attachment attachment, U32 textureId, U32 level)
		{
			glNamedFramebufferTexture(m_id, vx::gl::getAttachment(attachment), textureId, level);
		}

		void Framebuffer::attachTextureLayer(Attachment attachment, U32 textureId, U32 level, U32 layer)
		{
			glNamedFramebufferTextureLayer(m_id, vx::gl::getAttachment(attachment), textureId, level, layer);
		}
	}
}